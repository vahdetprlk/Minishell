
#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>

void	ft_free_list(char **list)
{
	int	i;

	i = 0;
	while (list[i])
	{
		free(list[i]);
		i++;
	}
	free(list);
}

char	**ft_lex(char *prompt)
{
	char	**cmd;

	cmd = ft_split(prompt);
	if (!cmd)
	{
		perror("Malloc failed");
		return (NULL);
	}
	return (cmd);
}

char	**ft_prompt_hook(char *prompt)
{
	char	**cmd;

	cmd = ft_lex(prompt);
	if (!cmd)
		return (NULL);
	return (cmd);
}

int	main(void)
{
	char	*prompt;
	char	**cmd_list;

	while (1)
	{
		prompt = readline("minishell$ ");
		if (!prompt)
			break ;
		cmd_list = ft_prompt_hook(prompt);
		if (!cmd_list)
		{
			free (prompt);
			break ;
		}
		int i = 0;
		while (cmd_list[i])
		{
			printf("%s\n", cmd_list[i]);
			i++;
		}
		free(prompt);
		ft_free_list(cmd_list);
	}
}
