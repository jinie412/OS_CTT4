#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

#define MAXLINE 512

int main(int argc, char *argv[]) {
    if (argc >= MAXARG) {
        printf("A number of arguments is greater than MAXARG.\n");
        exit(0);
    }
    char buffer[MAXLINE];

    char *args[MAXARG];

    for (int i = 1; i < argc; i++) {
        args[i - 1] = argv[i];
    }
    int numberOfBytesRead = -1;

    while ((numberOfBytesRead = read(0, buffer, sizeof(buffer))) > 0) {
        int startLine = 0;
        for (int i = 0; i < numberOfBytesRead; i++) {
            if (buffer[i] == '\n') {
                buffer[i] = 0;

                args[argc - 1] = &buffer[startLine];
                args[argc] = 0;

                if (fork() == 0) {
                    exec(argv[1], args);
                    printf("xargs: exec %s failed\n", args[0]);
                    exit(0);
                }
                wait(0);
                startLine = i + 1;
            }
        }
       
    }

    if (numberOfBytesRead < 0) {
        printf("xargs: read error\n");
        exit(0);
    }

    exit(0);
}
