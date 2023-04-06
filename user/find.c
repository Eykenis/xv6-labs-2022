#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  *(buf + strlen(p)) = 0;
  return buf;
}

void find(char* path, char* target) {
    char buf[512];
    char* p;
    int fd;
    struct dirent de;
    struct stat st;
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch(st.type) {
        case T_FILE:
            // printf("%s is a file, fmtname: %s, target: %s\n", path, fmtname(path), target);
            if (!strcmp(target, fmtname(path))) {
                printf("%s\n", path);
            }
            break;
        case T_DIR:
        // printf("%s is a directory\n", path);
            if (strlen(path) + DIRSIZ + 1 + 1 > sizeof(buf)) {
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0) continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
                    continue;
                find(buf, target);
            }
            break;
        default:
            break;
    }
    close(fd);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: find directory filename..\n");
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}