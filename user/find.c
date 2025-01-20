#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *name)
{
    char buf[512];
    char *p;
    
    struct dirent de;
    struct stat st;

    int fd = open(path,0);

    if (fd < 0) {
        printf("find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        printf("find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) 
    {
        case T_FILE:
            if (strcmp(path + strlen(path) - strlen(name), name) == 0) 
            {
                printf("%s\n", path);
            }
            break;

        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) 
            {
                printf("find: path too long\n");
                break;
            }

            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';

            while (read(fd, &de, sizeof(de)) == sizeof(de)) 
            {
                if (de.inum == 0)
                    continue;

                if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                    continue;

                memmove(p, de.name, strlen(de.name));
                p[strlen(de.name)] = 0;

                find(buf, name);
            }
            break;
    }
    
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Usage: find <path> <name>\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}
