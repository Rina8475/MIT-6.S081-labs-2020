#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

#define MAXPATH 512

void find(char *path, char *filename, int pathlen, int buflen);
int Open(const char *fname, int flag);
void Stat(char *fname, struct stat *st);

int main(int argc, char *argv[]) {
    struct stat st;
    int pathlen;
    char path[MAXPATH];

    if (argc != 3) {
        exit(1);
    }
    Stat(argv[1], &st);
    if (st.type == T_DIR) {
        strcpy(path, argv[1]);
        pathlen = strlen(path);
        if (path[pathlen] != '/') {
            strcpy(path + pathlen, "/");
        }
        find(path, argv[2], pathlen + 1, MAXPATH);
    } else if (st.type == T_FILE && strcmp(argv[1], argv[2]) == 0) {
        /* if input name of file */
        printf("%s\n", argv[1]);
    }
    exit(0);
}

/* find - find a file in directory PATH whose name is FILENAME */
void find(char *path, char *filename, int pathlen, int buflen) {
    int fd;
    char *p;
    struct stat st;
    struct dirent de;

    p = path + pathlen;
    /* open this directory and read it */
    fd = Open(path, 0);
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0) {
            continue;
        } else if (strcmp(de.name, ".") == 0 ||         /* pass dir . and .. */
                strcmp(de.name, "..") == 0) {
            continue;
        }
        /* generate the name of file */
        if (pathlen + 1 + strlen(de.name) + 1 > buflen) {
            fprintf(2, "find: name %s %s too long\n", path, de.name);
            exit(1);
        }
        strcpy(p, de.name);
        Stat(path, &st);
        /* check whether this file is dir */
        if (st.type == T_DIR) {
            strcpy(p + strlen(p), "/");
            find(path, filename, pathlen + strlen(de.name) + 1, buflen);
        } else if (st.type == T_FILE && strcmp(de.name, filename) == 0) {
            printf("%s\n", path);
        }
    }
}

/* Open - wrapped function */
int Open(const char *fname, int flag) {
    int fd;

    if ((fd = open(fname, flag)) < 0) {
        exit(1);
    }
    return fd;
}

/* Stat - wrapped function */
void Stat(char *fname, struct stat *st) {
    if (stat(fname, st) < 0) {
        exit(1);
    }
}