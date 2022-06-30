//
// Created by SGX on 2022/6/30.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define SENDSIZE 16

int main(int argc, char** argv) {

    //Parent send byte to child, child receive and send back
    int fd[2], pid;
    char buf[SENDSIZE];
    pipe(fd);
    pid = fork();
    if (pid == 0) {     //Child
        read(fd[0], buf, 4);
        close(fd[0]);
        printf("%d: received %s\n", getpid(), buf);
        write(fd[1], "pong", 4);
        close(fd[1]);
        exit(0);
    } else {
        write(fd[1], "ping", 4);
        close(fd[1]);

        wait(0);            //wait for the child process terminted
            read(fd[0], buf, 4);
        printf("%d: received %s\n", getpid(), buf);
        exit(0);
    }

}

