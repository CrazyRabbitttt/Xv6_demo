//
// Created by SGX on 2022/3/30.
//

//passed by int
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define  MAXPrime 35


void Primes(int read_fd) {
    int First;
    if (read(read_fd, &First, sizeof (int)) == 0) {
        exit(0);
    }
    printf("prime %d\n", First);

    //创建管道继续进行操作
    int fd[2];
    pipe(fd);
    if (!fork()) {
        close(fd[0]);
        int value;
        while (read(read_fd, &value, sizeof (int)) != 0) {
            if (value % First) {
                write(fd[1], &value, sizeof (int));
            }
        }

        close(fd[1]);
    } else {
        close(fd[1]);
        Primes(fd[0]);
    }


}

int main(int argc, char** argv) {
    int curnumber = 2;
    int fd[2];
    pipe(fd);       //create pipe

    if (!fork()) {
        close(fd[0]);       //关闭读取的一端
        for (int i = curnumber; i <= MAXPrime; i++) {           //Init
            write(fd[1], &i, sizeof(int));
        }
        close(fd[1]);
    } else {
        close(fd[1]);
        Primes(fd[0]);
    }

    exit(0);
}





