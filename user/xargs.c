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
    char * xargv[MAXARG] ;

    int index = 0;
    for (int i = 1; i < argc; i++) {
        xargv[index] = argv[i];
        index ++;
    }

    char *p = buf;
    //对于按照\n进行分隔的参数作为命令行参数

    for (int i = 0; i < MSSIZE; i++) {
        if (buf[i] == '\n') {
            if (!fork()) {  //child
                buf[i] = 0;
                xargv[index++] = p;
                xargv[index] = 0;
                excv(xargv[0], xargv);

                exit(1);        //if child run this, means error
            }else {     //parent
                p = &buf[i  + 1];
                wait(0);
            }
        }
    }



}

