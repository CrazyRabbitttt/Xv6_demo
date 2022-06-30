//
// Created by SGX on 2022/3/30.
//

//passed by int
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define  MAXPrime 35

#define First 2

void GotAndPass(int curnumber, int *fd) {
    //读取、关闭读取、写入、关闭写入
    int pid = fork();
    if (pid == 0) {
        int n, Next = -1;
        int num;
        while((n = read(fd[0], &num, sizeof (int))) > 0) {      //read from pipe
            //传送的参数fd
            if (Next == -1) {Next = num;}
            write(fd[1], &num, sizeof (int));
        }
        //传送过去之后，只是将管道穿过去了
        close(fd[0]);
        close(fd[1]);
        GotAndPass(Next, fd);
    } else {            //父进程
        close(fd[0]);
        //进行传递
        printf("prime %d\n", curnumber);
        for (int i = curnumber + 1; i <= MAXPrime; i++) {
            if (i % curnumber) {
                write(fd[1], &i, sizeof (int));
            }
        }
        //close port
        close(fd[1]);
    }
}


int main(int argc, char** argv) {
    int curnumber = First;
    int fd[2];
    pipe(fd);       //create pipe
    for (int i = First; i <= MAXPrime; i++) {           //Init
        write(fd[1], &i, sizeof(int));
    }

    printf("-------------\n");

    GotAndPass(curnumber, fd);
    exit(0);
}





