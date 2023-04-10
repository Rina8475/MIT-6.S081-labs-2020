#include "kernel/types.h"
#include "user/user.h"

int Write(int fd, void *buf, int size);
int Read(int fd, void *buf, int size);

int main(void) {
    char c;
    int pid, p2c[2], c2p[2];

    c = 'a';
    pipe(p2c);
    pipe(c2p);
    if ((pid = fork()) == 0) {      /* child */
        close(p2c[1]);
        close(c2p[0]);
        Read(p2c[0], &c, 1);
        printf("%d: received ping\n", getpid());
        Write(c2p[1], &c, 1);
        exit(0);
    } 
    close(p2c[0]);
    close(c2p[1]);
    Write(p2c[1], &c, 1);         /* send a byte */
    Read(c2p[0], &c, 1);          /* receive a byte */
    printf("%d: received pong\n", getpid());
    exit(0);
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
