#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int ft_create_file(char *file) {
    int fd;

    fd = open(file, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    return fd;
}

int main() {
    int fd[3];

    char *command[] = {"ls", ">", "vahdet", ">", "burak", "", "cemal", NULL};
    int i = 0;
    int j = 0;

    while (command[i]) {
        if (strcmp(command[i], ">") == 0) {
            fd[j] = ft_create_file(command[i + 1]);
            printf("fd = %d\n", fd[j]);
            j++;
        }
        i++;
    }

    char str[10000];

    write(fd[1], "burak", 5);
    close(fd[1]);

    fd[1] = open("burak", O_RDONLY);
    read(fd[1], str, 5);
    
    write(fd[0], str, 5);
    close(fd[0]);

    return 0;
}
