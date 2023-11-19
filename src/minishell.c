
#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
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

typedef enum e_token_type {
	COMMAND,
	HEREDOC,
	APPEND,
	OR,
	AND,
	PIPE,
	INPUT_REDIRECT,
	OUTPUT_REDIRECT,
	LPHARANTHESIS,
	RPHARANTHESIS
}	t_token_type;

typedef struct s_Token {
	char			*value;
	t_token_type	type;
}	t_token;

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
			ft_free_list((char **)token_struct_list);
			return (0);
		}
		new_token->value = token_list[i];
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
	return (1);
}

void	ft_token_validation(t_token **token_struct_list)
{
/* 	int i = 0;
	while (token_struct_list[i])
	{
		printf("%d :: ", token_struct_list[i]->type);
		printf("%s\n", token_struct_list[i]->value);
		i++;
	} */
}

void	ft_prompt_hook(char *prompt)
{
	t_token	**token_struct_list;
	char	**token_list;
	int		lst_len;

	token_list = ft_tokenization(prompt);
	if (!token_list)
	{
		perror("Malloc failed");
		free(prompt);
		return ;
	}
	free(prompt);
	lst_len = ft_listlen(token_list);
	token_struct_list = (t_token **)ft_calloc(lst_len + 1, sizeof(t_token *));
	if (!token_struct_list)
	{
		perror("Malloc failed");
		ft_free_list(token_list);
		return ;
	}
	if (!ft_token_classification(token_list, token_struct_list))
		return ;
	ft_token_validation(token_struct_list);
	ft_free_list((char **)token_struct_list);
	ft_free_list(token_list);
}

int	main(void)
{
	char	*prompt;

	while (1)
	{
		prompt = readline("minishell$ ");
		if (!prompt)
			break ;
		ft_prompt_hook(prompt);
	}
}
