#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    char buf[320];

    // copy args to command ptr
    char *command[MAXARG];
    argc--;
    for (int i = 0; i < argc; ++i) {
        command[i] = argv[i + 1];
    }

    // read std input to buf, and append buf
    int input_size, argptr;
    while ((input_size = read(0, buf, sizeof(buf))) > 0) {
        argc++, argptr = 0;
        command[argc - 1] = malloc(64 * sizeof(char));
        for (int i = 0; i < input_size; ++i) {
            char ch = buf[i];
            if (ch == ' ') {
                argc++, argptr = 0;
                command[argc - 1] = malloc(64 * sizeof(char));
                if (argc > MAXARG) {
                    fprintf(2, "Too many args to xargs!\n");
                    exit(1);
                }
            }
            else if (ch == '\n') {
                break;
            }
            else {
                command[argc - 1][argptr++] = ch;
            }
        }
    }

    // use childproc to execute the command follows xargs
    if (!fork()) {
        exec(argv[1], command);
    }
    else {
        wait(0);
    }
    exit(0);
}