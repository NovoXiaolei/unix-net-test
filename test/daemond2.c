#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
    pid_t pid;
    pid = fork();
    if (pid > 0) {
        return 0;
    }
    setsid();
    int ret = chdir("/home/xia0lei/");
    if (ret < 0){
        perror("chdir error");
        exit(1);
    }

    umask(0002);
    close(STDIN_FILENO);
    open("/dev/null", O_RDWR);
    dup2(0, STDOUT_FILENO);
    dup2(0, STDERR_FILENO);

    while(1) {
    }
    return 0;
}
