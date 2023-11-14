
#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>

char	**ft_lex(char *prompt)
{
	char	**cmd;

	cmd = ft_split(prompt); // leaks kontrolu yapilacak (ft_split.c) 
	int i = 0;
	while (cmd[i])
	{
		printf("%s\n", cmd[i]);
		i++;
	}
	return (cmd);
}

void	ft_prompt_hook(char *prompt)
{
	char	**cmd;
	//char	**cmd_list;

	cmd = ft_lex(prompt);
	//cmd_list = ft_tokenizer(cmd);
}

int main(void)
{
	char	*prompt;

	while (1)
	{
		prompt = readline("minishell$ ");
		if (!prompt)
			break;
		ft_prompt_hook(prompt);
	}
}
