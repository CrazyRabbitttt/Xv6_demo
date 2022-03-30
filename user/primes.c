//
// Created by SGX on 2022/3/30.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXNUM 35

#define FIRST_PRIME 2

int
generate_natual() {
    int fd[2];
    pipe(fd);
    int pid = fork();
    if (pid == 0) {
        close(fd[1]);
        return fd[0];
    }else {
        for (int i = FIRST_PRIME; i <= MAXNUM; i++) {
            write(fd[1], &i, sizeof (int));
        }
        close(fd[1]);
        exit(0);
    }
}

int
prime_handle(int in_fd, int prime) {
    //to guolv prime number
    int fd[2];
    int num;            //store the number you read

    //主要是关闭 写的端口，防止子进程阻塞？
    int pid = fork();
    if (pid == 0) {
        close(in_fd);
        close(fd[1]);
        return fd[0];
    }else {
        while (read(in_fd, &num, sizeof (int))) {
            if (num % prime) {
                write(fd[1], &num, sizeof (int));
            }
        }
        close(in_fd);
        close(fd[1]);
        exit(0);
    }
}

int main()
{
    int prime;
    int in = generate_natual();     //the number of read descripter
    while (read(in, &prime, sizeof (int))) {
        printf("prime %d\n", prime);
        in = prime_handle(in, prime);
    }
}


