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

    //if not the dir
    if (st.type != T_DIR) {
        printf("Error: The argument %s not a dir\n", dirname);
        return;
    }


    //判断所有的文件fmt 是否是 equal with filename
    if(strlen(dirname) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("find: path too long\n");
        return;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){         //read from dir
        if(de.inum == 0)
            find(de.name, filename);                        //concurrent
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        if (strcmp(fmtname(buf), filename) == 0) {
            printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
        }
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
