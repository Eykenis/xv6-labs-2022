#include "kernel/types.h"
#include "user/user.h"

int main(int argc, int *argv[]) {
    int fd[2];
    if (pipe(fd) < 0) {
        fprintf(2, "Pipe creation failed\n");
        exit(1);
    }
    int pid;
    if (!(pid = fork())) {
        fprintf(1, "%d: received ping\n", getpid());
        write(fd[1], "x", 1);
    }
    else {
        wait(0);
        fprintf(1, "%d: received pong\n", getpid());
        char *buf = malloc(sizeof(char));
        read(fd[0], buf, 1);
        fprintf(1, "message %s received\n", buf);
        free(buf);
    }
    close(fd[0]);
    close(fd[1]);
    exit(0);
}