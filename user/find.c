//
// Created by SGX on 2022/3/30.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char *
fmtname(char *path) {
    char *p;
    while (p = path + strlen(path); *p != '/' && p >= path; p--)
        ;
    p++;
    return p;
}

void find(char *path, char *target)
{
    char buf[505], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if (!strcmp(fmtname(path), target)) {
        printf("%s\n", path);
        return ;
    }

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: can't open %s\n", path);
        return ;
    }
    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: can't stat %s", path);
        close(fd);
        return ;
    }

    if (st.type != T_DIR) {
        close(fd);
        return ;
    }

    //DIR
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("ls: path too long\n");
        close(fd);
        return ;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof (de)) == sizeof (de)) {
        if (de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        //path = oldpath + filename

        if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
            continue;
        find(buf, target);
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(2, "usage: find <dirname> <filename>");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}