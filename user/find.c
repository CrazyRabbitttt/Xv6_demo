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



void
find(char* dirname, char* filename) {
    //find dirname filename
    //first : open file
    int fd;
    char buf[512], *p;
    struct  dirent de;      //dir status
    struct  stat st;        //dile stat

    //open dir
    if ((fd = open(dirname, 0)) < 0) {
        fprintf(2, "find: can not open dir %s\n", dirname);
        return;
    }

    // get the stat of the dir
    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", dirname);
        close(fd);
        return;
    }


    switch (st.type) {
        case T_FILE:        //type is file
            if (strcmp(filename, dirname) == 0)
                printf("%s\n", dirname);
            break;
        case T_DIR:
            if(strlen(dirname) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, dirname);
            p = buf+strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0 || de.inum == 1 || strcmp(dirname, ".") == 0 || !strcmp(dirname, "..") == 0)
                    continue;
                memmove(p, de.name, strlen(de.name));	//将文件名追加到路径，递归find
                p[strlen(de.name)] = '\0';
                find(buf, filename);
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
