//
// Created by SGX on 2022/3/31.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernrl/param.h"       //MAXPATH 128

#define MESSAGE 16

char
int main(int argc, char * argv[]) {
    //从管道中获取的标准输入作为参数
    char buf[MESSAGE];
    read(0, buf, MESSAGE);      //从标准输入中获得参数  -> buf
    printf("得到的标准化的输入： %s\n", buf);
    //那么argv[0] : 就是你命令行参数
    //需要执行命令，参数就是buf中保存的参数

}

