#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
    int second;
    if (argc != 2){
        fprintf(2, "Usage: sleep seconds\n");
        exit(1);
    }
    second = atoi(argv[1]);
    fprintf(2, "sleeping %d seconds\n", second);
    sleep(second);
    exit(0);
}