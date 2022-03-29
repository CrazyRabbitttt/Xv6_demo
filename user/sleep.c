//
// Created by 邵桂鑫 on 2022/3/29.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(2, "usage : sleep seconds\n");
        exit(0);
    }
    sleep(atoi(argv[1]));
    exit(0);
}