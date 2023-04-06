#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        char* str;
        str = "Usage: sleep sleeptime..\n";
        write(1, str, strlen(str));
        exit(0);
    }
    int time = atoi(argv[1]);
    sleep(time);
    exit(0);
}