//
// Created by SGX on 2022/3/31.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"       //MAXARG


#define MESSAGE 26


int main(int argc, char * argv[]) {
   //从管道中获得命令行参数（从标准输入读取）
   char buf[MESSAGE];
   read(0, buf, MESSAGE);

   char *xargv[MAXARG];
   int index = 0;
   for (int i = 1; i < argc; i++) {
       xargv[index] = argv[i];
       index++;
   }

   //通过回车行进行分隔

   char *p = buf;
   for (int i = 0; i < MESSAGE; i++) {
       if(buf[i] == '\n') {
           if (!fork()) {
               buf[i] = 0;
               xargv[index++] = p;
               xargv[index++] = 0;
               exec(xargv[0], xargv);
               exit(1);
           }else {      //parent
               p = &buf[i + 1];
               wait(0);
           }
       }
   }

   wait(0);
    exit(0);
}

