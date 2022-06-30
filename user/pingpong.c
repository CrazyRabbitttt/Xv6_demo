//
// Created by SGX on 2022/6/30.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define SENDSIZE 16

int main(int argc, char** argv) {

    //Parent send byte to child, child receive and send back
    int fd[2];
    char sendbuf[SENDSIZE];
    char recvbuf[SENDSIZE];
    pid_t pid;
    sendbuf = "Ping";
    if (pipe(fd) < 0) {
        printf("create pipe error, quit\n");
        exit(1);
    }
    if ((pid = fork()) < 0) {
        printf("create process error, quit\n");
        exit(1);
    } else if (pid > 0) {       //Parent
        write(fd[1], sendbuf, SENDSIZE);        //send message to pipe[1], and

        sleep(1);
        int n = read(fd[0], sendbuf, SENDSIZE);
        printf("%d: received pong\n", getpid());
    } else {                    //Child
        int n = read(fd[0], recvbuf, SENDSIZE);
        if (n) {
            printf("%d: received ping\n", getpid());
            write(fd[1], sendbuf, SENDSIZE);            //send back
        }
    }

}

