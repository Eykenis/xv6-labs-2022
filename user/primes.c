#include "kernel/types.h"
#include "user/user.h"

void test_prime(int fd) {
    int num, p;
    int rd;
    if ((rd = read(fd, &num, 4)) && rd != 4) {
        // end of recursion
        fprintf(2, "pipe read is not in int type\n");
        exit(0);
    }
    else if (!rd) exit(0);

    fprintf(1, "prime %d\n", num);
    int son_fd[2];

    pipe(son_fd);
    if (!fork()) { // child
        close(fd);
        close(son_fd[1]);
        test_prime(son_fd[0]);
    }
    else {
        close(son_fd[0]);
        while ((rd = read(fd, &p, 4))) {
            if (rd != 4) {
                fprintf(2, "pipe read is not in int type\n");
                exit(1);
            }
            if (p % num) {
                write(son_fd[1], &p, 4);
            }
        }
        close(son_fd[1]);
        close(fd);
        wait(0);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    int fd[2];
    pipe(fd);
    int i, n = 35;
    for (i = 2; i <= n; ++i) { // initialize
        write(fd[1], &i, 4);
    }
    close(fd[1]);
    test_prime(fd[0]);
    exit(0);
}