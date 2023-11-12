
#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>

void	*ft_realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	new_ptr = ft_calloc(size, sizeof(char));
	if (!new_ptr)
	{
		free(ptr);
		return (NULL);
	}
	ft_strlcpy(new_ptr, ptr, size);
	free(ptr);
	return (new_ptr);
}

void ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
}

int	ft_single_quote(char *prompt)
{
	int	len;

	len = 1;
	while (prompt[len])
	{
		if (prompt[len] == '\'')
			break;
		len++;
	}
	if (prompt[len] == '\'')
		len++;
	return (len);
}

int	ft_double_quote(char *prompt)
{
	int	len;

	len = 1;
	while (prompt[len])
	{
		if (prompt[len] == '\"')
			break;
		len++;
	}
	if (prompt[len] == '\"')
		len++;
	return (len);
}

int	ft_lex_quote(char *prompt, char **cmd)
{
	int		len;

	len = 0;
	if (prompt[len] == '\'')
		len = ft_single_quote(prompt);
	else
		len = ft_double_quote(prompt);
	*cmd = ft_substr(prompt, 0, len + 1);
	if (!cmd)
		return (0);
	return (len);
}


char	**ft_lex(char *prompt)
{
	int		i;
	int		j;
	int		len;
	char	**cmd;

	cmd = (char **)ft_calloc(2, sizeof(char *));
	if (!cmd)
		return (NULL);
	len = 0;
	j = 0;
	i = 0;
	while (prompt[i])
	{
		if (prompt[i] == '\'' || prompt[i] == '\"')
		{
			i += ft_lex_quote(prompt + i, cmd + j);
			if (i > 0)
				cmd = (char **)ft_realloc(cmd, sizeof(char *) * (j + 2));
				if (!cmd)
					return (NULL);
			j++;
		}
		else
		{
			while (prompt[i + len] && prompt[i + len] != '\'' && prompt[i + len] != '\"')
				len++;
			cmd[j] = ft_substr(prompt, i, len);
			if (!cmd[j])
			{
				ft_free_tab(cmd);
				return (NULL);
			}
			i += len;
		}
	}
	return (cmd);
}



void	ft_prompt_hook(char *prompt)
{
	char	**cmd;
	//char	**cmd_list;

	cmd = ft_lex(prompt);
	free (prompt);
	printf("%s\n", cmd[0]);
	printf("%s\n", cmd[1]);
	printf("%s\n", cmd[2]);
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
