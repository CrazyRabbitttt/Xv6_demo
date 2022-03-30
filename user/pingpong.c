//
// Created by SGX on 2022/3/30.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUFSIZE 16

int main()
{
    int pid;
    int fd[2];
    pipe(fd);
    char buf[BUFSIZE];
    pid = fork();
    if (pid == 0) {     /*child*/
        read(fd[0], buf, 4);
        close(fd[0]);
        printf("%d: received %s\n", getpid(), buf);
        write(fd[1], "pong", 4);
        close(fd[1]);
        exit(0);
    }else {
        write(fd[1], "ping", 4);
        close(fd[1]);
        wait(0);        //wait for child end...

        read(fd[0], buf, 4);
        printf("%d: received %s\n", getpid(), buf);
        close(fd[0]);
        exit(0);
    }
}

