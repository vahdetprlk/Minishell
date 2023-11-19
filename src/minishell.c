
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

typedef enum e_TokenType {
	HEREDOC,
	APPEND,
	OR,
	AND,
	PIPE,
	INPUT_REDIRECT,  // '<' için
	OUTPUT_REDIRECT, // '>' için
	COMMAND
} TokenType;

typedef struct s_Token {
	char *value;
	TokenType type;
} Token;

void ft_token_classification(char **token_list, Token **token_struct_list)
{
	int i = 0;
	while (token_list[i])
	{
		Token *new_token = malloc(sizeof(Token));
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
		else
			new_token->type = COMMAND;
		token_struct_list[i] = new_token;
		i++;
	}
}

void	ft_prompt_hook(char *prompt)
{
	char	**token_list;

	token_list = ft_tokenization(prompt);
	if (!token_list)
	{
		perror("Malloc failed");
		ft_free_list(token_list);
	}
	ft_token_classification(token_list);
	int i = 0;
	while (token_list[i])
	{
		printf("%s\n", token_list[i]);
		i++;
	}
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
		free(prompt);
	}
}
