#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAXLEN  512
#define NULL    ((void *) 0)

int getline(int fd, char *buf, int buflen);
int Fork(void);
int Read(int fd, void *buf, int size);

int main(int argc, char *argv[]) {
    char line[MAXLEN];
    int pid;
    
    if (argc < 2) {
        exit(1);
    }
    
    for (int i = 1; i < argc; i += 1) {
        argv[i-1] = argv[i];
    }
    while (getline(0, line, MAXLEN) > 0) {
        argv[argc-1] = line;
        if ((pid = Fork()) == 0) {  /* child */
            exec(argv[0], argv);
            exit(1);        /* if exec error */
        }
        wait(NULL);
    }
    exit(0);
}

/* getline - get a line from file FD, and write it into BUF 
 * return the length of line readed into BUF, excluding '\0'
 * the return line do not include '\n' */
int getline(int fd, char *buf, int buflen) {
    int idx;
    char c;

    /* P.S. this will be very slow, because i only read one byte when i 
    call system call READ, a faster way is to build a buffer */
    for (idx = 0; idx < buflen-1 && Read(fd, &c, 1) > 0; idx += 1) {
        if (c == '\n') {
            break;
        }
        buf[idx] = c;
    }
    buf[idx] = '\0';
    return idx;
}

/* Fork - wrapped function */
int Fork(void) {
    int pid;

    if ((pid = fork()) < 0) {
        exit(1);
    }
    return pid;
}

/* Read - wrapped function */
int Read(int fd, void *buf, int size) {
    int cnt;

    if ((cnt = read(fd, buf, size)) < 0) {
        exit(1);
    }
    return cnt;
}