#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>


void	ft_prompt_check(char *prompt)
{
	char **splitted_prompt;

	splitted_prompt = ft_split(prompt, ' ');
	int i = 0;
	while (splitted_prompt[i])
	{
		printf("%s\n", splitted_prompt[i]);
		i++;
	}

}

void	ft_prompt_hook(char *prompt)
{
	ft_prompt_check(prompt);
	free(prompt);
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
