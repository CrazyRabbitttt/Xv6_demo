//
// Created by SGX on 2022/3/30.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUFSIZE 16

int
main(int argc, char *argv[])
{
    int fd[2];
    int pid, curPid;
    //make a pipe
    pipe(fd);                   //pass value fd[0]  fd[1], represent the receive and send

    char buf[BUFSIZE];
    //first : the child process receive the message from parent,
    //then  : the parent received the message from son
    pid = fork();
    if (pid == 0) {      //child
        read(fd[0], buf, 10);
        curPid = getpid();
        printf("%d: received%s\n", curPid, buf);
        write(fd[1], "pong", 4);
        exit(0);
    }else  { //parent
        write(fd[1], "ping", 4);
        wait(NULL);
        read(fd[0], buf, 4);
        curPid = getpid();
        printf("%d: received %s\n", curPid, buf);
    }
}

