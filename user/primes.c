#include "kernel/types.h"
#include "user/user.h"

#define ENDNUM  35
#define NULL    ((void *) 0)

void child_do(int fd);
int Fork(void);
int Write(int fd, void *buf, int size);
int Read(int fd, void *buf, int size);

int main(void) {
    int cnt, pid, p[2];

    cnt = 2;
    pipe(p);
    if ((pid = Fork()) == 0) {      /* child */
        close(p[1]);
        child_do(p[0]);
    }
    close(p[0]);        /* close read */
    printf("prime %d\n", cnt);
    for (int i = cnt+1; i <= ENDNUM; i += 1) {
        if ((i % cnt) != 0) {
            Write(p[1], &i, sizeof(int));
        }
    }
    close(p[1]);        /* close write before wait */
    wait(NULL);
    exit(0);
}

/* child_do - the operations every children will do */
void child_do(int fd) {
    int fval, val, pid, p[2];

    if (Read(fd, &fval, sizeof(int)) == 0) {    /* base case of recursion */
        exit(0);
    }
    /* if read the first value */
    printf("prime %d\n", fval);
    /* creat a child and a pipe */
    pipe(p);
    if ((pid = Fork()) == 0) {      /* child */
        close(p[1]);
        child_do(p[0]);
    }
    close(p[0]);        /* close read */
    while (Read(fd, &val, sizeof(int)) > 0) {
        if ((val % fval) != 0) {
            Write(p[1], &val, sizeof(int));
        }
    }
    close(p[1]);        /* close write before wait */
    wait(NULL);
    exit(0);
}

/* Fork - wrapped function */
int Fork(void) {
    int pid;

    if ((pid = fork()) < 0) {
        exit(1);
    }
    return pid;
}

/* Write - wrapped function */
int Write(int fd, void *buf, int size) {
    int cnt;

    if ((cnt = write(fd, buf, size)) < 0) {
        exit(1);
    }
    return cnt;
}

/* Read - wrapped function */
int Read(int fd, void *buf, int size) {
    int cnt;

    if ((cnt = read(fd, buf, size)) < 0) {
        exit(1);
    }
    return cnt;
}
