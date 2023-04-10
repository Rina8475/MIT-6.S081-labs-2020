#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int cnt;

    if (argc < 2) {
        fprintf(2, "%s: missing operand\n", argv[0]);
        exit(1);
    }

    cnt = atoi(argv[1]);
    sleep(cnt);
    exit(0);
}