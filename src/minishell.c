#include <unistd.h>
#include <stdio.h>

int main() {
    char buffer[2000];
    char *argv[] =  {"ls", NULL};
    int fd[2];
    pipe(fd);
    int pid = fork();
    if (pid == 0)
    {
        close(3);
        dup2(4, STDOUT_FILENO);
        execve("/bin/ls", argv, NULL);
    }
    else
    {
        close(4);
        read(3, buffer, 300);
        int fd2[2];
        pipe(fd2);
        pid = fork();
        if (pid == 0)
        {
            close(5);
            char *argvx[] = {"echo", buffer, NULL};
            execve("/bin/echo", argvx , NULL);
        }
    }
}