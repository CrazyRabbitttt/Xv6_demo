//
// Created by SGX on 2022/3/31.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"       //MAXPATH 128

#define MESSAGE 16


int main(int argc, char * argv[]) {
    //从管道中获取的标准输入作为参数
    char buf[MESSAGE];
    read(0, buf, MESSAGE);      //从标准输入中获得参数  -> buf
//    printf("得到的标准化的输入： %s\n", buf);
    //那么argv[0] : 就是你命令行参数
    //需要执行命令，参数就是buf中保存的参数

    //  第一个参数就是需要执行的命令
    char * xargv[] ;

    char * command = argv[0];          //命令行参数
    int index = 1;
    for (index; index < argc; index++) {
        xargs[index - 1] = argv[index];         //将参数加到xargv中去
    }
    //the end of index : argc
    xargs[index] = buf;     //最后的应该就是从管道得到的参数

    if (!fork()) {
        excv(command, xargv);
        exit(0);
    }

    wait();
    exit(0);
}

