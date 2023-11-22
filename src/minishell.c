
#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

int	ft_listlen(char **list)
{
	int	i;

	i = 0;
	while (list[i])
		i++;
	return (i);
}

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

void	ft_free_struct_list(t_token **token)
{
	int	i;

	i = 0;
	while (token[i])
	{
		free(token[i]->value);
		free(token[i]);
		i++;
	}
	free(token);
}



int	ft_token_classification(char **token_list, t_token **token_struct_list)
{
	int		i;
	t_token	*new_token;

	i = 0;
	while (token_list[i])
	{
		new_token = (t_token *)ft_calloc(1, sizeof(t_token));
		if (!new_token)
		{
			perror("Malloc failed");
			ft_free_list(token_list);
			ft_free_struct_list(token_struct_list);
			return (1);
		}
		new_token->value = ft_strdup(token_list[i]);
		if (!new_token->value)
		{
			perror("Malloc failed");
			ft_free_list(token_list);
			ft_free_struct_list(token_struct_list);
			return (1);
		}

		if (ft_strncmp(token_list[i], "<<", 2) == 0)
			new_token->type = HEREDOC;
		else if (ft_strncmp(token_list[i], ">>", 2) == 0)
			new_token->type = APPEND;
		else if (ft_strncmp(token_list[i], "||", 2) == 0)
			new_token->type = OR;
		else if (ft_strncmp(token_list[i], "&&", 2) == 0)
			new_token->type = AND;
		else if (token_list[i][0] == '|')
			new_token->type = PIPE;
		else if (token_list[i][0] == '>')
			new_token->type = OUTPUT_REDIRECT;
		else if (token_list[i][0] == '<')
			new_token->type = INPUT_REDIRECT;
		else if (token_list[i][0] == '(')
			new_token->type = LPHARANTHESIS;
		else if (token_list[i][0] == ')')
			new_token->type = RPHARANTHESIS;
		else
			new_token->type = COMMAND;
		token_struct_list[i] = new_token;
		i++;
	}
	return (0);
}

int	ft_token_validation(t_token **token_struct_list)// hatali calisiyor
{
	int	i;

	i = 0;
	while (token_struct_list[i])
	{
		if (token_struct_list[i]->type != COMMAND)
		{
			if (!token_struct_list[i + 1])
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
				return (2);
			}
			if (token_struct_list[i + 1]->type != COMMAND)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
				ft_putstr_fd(token_struct_list[i + 1]->value, STDERR_FILENO);
				ft_putstr_fd("'\n", STDERR_FILENO);
				return (2);
			}
		}
		i++;
	}
	return (0);
}


int	ft_prompt_hook(char *prompt)
{
	t_token	**token_struct_list;
	char	**token_list;
	int		lst_len;

	token_list = ft_tokenization(prompt);
	if (!token_list)
	{
		perror("Malloc failed");
		free(prompt);
		return (1);
	}
	free(prompt);
	lst_len = ft_listlen(token_list);
	token_struct_list = (t_token **)ft_calloc(lst_len + 1, sizeof(t_token *));
	if (!token_struct_list)
	{
		perror("Malloc failed");
		ft_free_list(token_list);
		return (1);
	}
	if (ft_token_classification(token_list, token_struct_list))
		return (1);
	ft_free_list(token_list);
	if (ft_token_validation(token_struct_list))
	{
		ft_free_struct_list(token_struct_list);
		return (2);
	}
	ft_free_struct_list(token_struct_list);
	return (0);
}

int	main(void)
{
	int		status;
	char	*prompt;

	status = 0;
	while (1)
	{
		prompt = readline("minishell$ ");
		if (!prompt)
			break ;
		add_history(prompt);
		status = ft_prompt_hook(prompt);
		printf ("%d\n", status);
	}
}
