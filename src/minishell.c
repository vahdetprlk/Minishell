#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int	ft_create_file(char *file)
{
	int	fd;

	fd = open(file, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	return (fd);
}

int main()
{
	int fd[3];

	char *command[] = {"ls", ">", "vahdet", ">", "burak", "", "cemal", NULL};
	int i = 0;
	int j = 0;
	while (command[i])
	{
		if (strcmp(command[i], ">") == 0)
		{
			fd[j] = ft_create_file(command[i + 1]);
			printf("fd = %d\n", fd[j]);
			j++;
		}
		i++;
	}
	char str[7];
	write(fd[0], "bahdet", 6);
	read(fd[0], str, 6);
	write(1, str, 6);


/* 	char *arg[] = {"ls", NULL};
	int pid = fork();
	if (pid > 0)
		wait(&pid);
	else
	{
		dup2(fd[1], STDOUT_FILENO);
		execve("/bin/ls", arg, NULL);
	} */
}
