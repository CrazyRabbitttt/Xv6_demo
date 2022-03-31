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
    //means open file by fmtname
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    return buf;
}

void find(char * path, char *target) {
    char buf[502], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if (!strcmp(fmtname(path), target)) {
        printf("%s\n", path);           //if fmt is equal, print the old path
        return ;
    }

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find can open %s, the fd is %d", path, fd);
        exit(1);
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cant stas %s\n", path);
        close(fd);
        return;
    }


    if (st.type == T_FILE) {
        close(fd);
        return ;        //not equal
    }

    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("ls: path too long\n");
        return ;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';

    while (read(fd, &de, sizeof (de)) == sizeof (de)) {
        if (de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;          //add path: old + filename

        if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
            continue;
        //now the file's path blow the dir already add to 'path'
        find(buf, target);
    }
    close(fd);

}

int
main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(2,"Usage : find <path> <filename>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}
