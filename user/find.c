//
// Created by SGX on 2022/3/30.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
fmtname(char *path)
{
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    return buf;
}

void cmp(char* a, char* b) {
    if (!strcmp(fmtname(a), b)) {
        printf("%s\n", a);
    }
}

void
find(char *path, char *target)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){	//根据路径打开文件
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){	//获取文件参数
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
        case T_FILE://该文件为文件时
            cmp(path, target);
            break;

        case T_DIR: //该文件为文件夹时
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                //若文件夹下文件数量为0，1或者该文件夹名字为"."或".."则不进入，防止套娃
                if(de.inum == 0 || de.inum == 1 || strcmp(de.name, ".")==0 || strcmp(de.name, "..")==0)
                    continue;
                memmove(p, de.name, strlen(de.name));	//将文件名追加到路径，递归find
                p[strlen(de.name)] = '\0';
                find(buf, target);
            }
            break;
    }
    close(fd);
}



int
main(int argc, char *argv[])
{
    //find dirPath filepath
    if (argc < 3) {
        printf("Usage find DirPath filePath\n");
        exit(0);
    }

    find(argv[1], argv[2]);

    exit(0);
}
