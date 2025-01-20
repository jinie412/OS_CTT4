#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int parent[2];
    int child[2];
    char *c;

    pipe(parent);
    pipe(child);

    if(fork() == 0)
    {   
        read(parent[0], &c, 1);
        printf("%d: received ping\n", getpid());
        write(child[1], c, 1);
        exit(0);
    }
    else   
    {
        c = "a";
        write(parent[1], c, 1);
        read(child[0], &c, 1);
        printf("%d: received pong\n", getpid());
    }
    exit(0);
}
