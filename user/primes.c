//
// Created by SGX on 2022/3/30.
//

//passed by int
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXNUM 35

#define FIRST_PRIME 2

int generate();

int filter(int in_fd, int prime);

int main()
{
    int prime;
    int in = generate();
    while (read(in, &prime, sizeof (int))) {
        printf("prime %d\n", prime);
        in = filter(in, prime);
    }
    exit(0);
}

int generate()
{
    int fd[2];

    pipe(fd);
    if (!fork()) {      //parent
        for (int i = FIRST_PRIME; i <= MAXNUM; i++) {
            write(fd[1], &i, sizeof (int));
        }
        close(fd[1]);
        exit(0);
    }
    //father will never got here
    //here is the child to do
    close(fd[1]);
    return fd[0];

}

int
filter(int in_fd, int prime)
{
    int value;
    int fd[2];
    pipe(fd);
    if (!fork()) {
        while (read(in_fd, &value, sizeof (int))) {
            if (value % prime) {
                write(fd[1], &value, sizeof (int));
            }
        }
        close(in_fd);
        close(fd[1]);


        exit(0);
    }

    //here also never have father process
    close(in_fd);
    close(fd[1]);
    return fd[0];

}









