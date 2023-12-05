
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
			free(new_token);
			ft_free_list(token_list);
			ft_free_struct_list(token_struct_list);
			return (1);
		}
		if (ft_strncmp(token_list[i], "<<", 2) == 0)
			new_token->type = HEREDOC;
		else if (ft_strncmp(token_list[i], ">>", 2) == 0)
			new_token->type = APPEND;
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
	return (0);
}

int	ft_token_validation(t_token **token_struct_list)
{
	int	i;

	i = 0;
	while (token_struct_list[i])
	{
		if (token_struct_list[i]->type == PIPE)
		{
			if (i == 0)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
				ft_putstr_fd(token_struct_list[i]->value, STDERR_FILENO);
				ft_putstr_fd("'\n", STDERR_FILENO);
				return (1);
			}
			else if (token_struct_list[i] && token_struct_list[i + 1])
			{
				if (token_struct_list[i + 1]->type == PIPE)
				{
					ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
					ft_putstr_fd(token_struct_list[i + 1]->value, STDERR_FILENO);
					ft_putstr_fd("'\n", STDERR_FILENO);
					return (1);
				}
				else
				{
					i++;
					continue ;
				}
			}
			else if (!token_struct_list[i + 1])
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
				return (1);
			}
		}
		if (token_struct_list[i]->type != COMMAND)
		{
			if (!token_struct_list[i + 1])
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
				return (1);
			}
			if (token_struct_list[i + 1]->type != COMMAND)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
				ft_putstr_fd(token_struct_list[i + 1]->value, STDERR_FILENO);
				ft_putstr_fd("'\n", STDERR_FILENO);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	ft_isalpha(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}
int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}

char	*ft_env_match(char *str, t_env *env_head)
{
	t_env	*temp;

	temp = env_head;
	while (temp)
	{
		if (ft_strcmp(str, temp->var_name) == 0)
			return (temp->var_value);
		temp = temp->next;
	}
	return (NULL);
}

char	*ft_dollar_sign_expansion(char *str, t_env *env_head, int *exit_status)
{
	int		i;
	int		j;
	int		len;
	char	*temp;
	char	*temp_free;
	char	*new_str;
	char	*env_value;

	len = 0;
	j = 0;
	i = 0;
	new_str = ft_strdup("");
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
		{
			i += 2;
			len = 2;
			env_value = ft_itoa(*exit_status);
			if (!env_value)
			{
				free(new_str);
				return (NULL);
			}
			temp_free = new_str;
			new_str = ft_strjoin(new_str, env_value);
			if (!new_str)
			{
				free(env_value);
				free(temp_free);
				return (NULL);
			}
			free(env_value);
			free(temp_free);
		}
		else if (str[i] == '$')
		{
			if (!str[i + 1])
				return (new_str);
			i++;
			j = i;
			len = 0;
			while (ft_isalnum(str[j]) || str[j] == '_')
			{
				j++;
				len++;
			}
			temp = ft_substr(&str[i], 0, len);
			if (!temp)
			{
				free(new_str);
				return (NULL);
			}
			i = j;
			env_value = ft_env_match(temp, env_head);
			if (!env_value)
				env_value = "";
			temp_free = new_str;
			new_str = ft_strjoin(new_str, env_value);
			if (!new_str)
			{
				free(temp);
				free(temp_free);
				return (NULL);
			}
			free(temp);
			free(temp_free);
		}
		else
		{
			j = i;
			while (str[j] && str[j] != '$')
				j++;
			temp = ft_substr(&str[i], 0, j - i);
			if (!temp)
			{
				free(new_str);
				return (NULL);
			}
			i = j;
			temp_free = new_str;
			new_str = ft_strjoin(new_str, temp);
			if (!new_str)
			{
				free(temp);
				free(temp_free);
				return (NULL);
			}
			free(temp);
			free(temp_free);
		}
	}
	return (new_str);
}
//env_head freelemeyi unutma fonksiyonlari boldukten sonra hangi fonksiyonlarda freelemek gerektigini bul
int	ft_quote_expansion(t_token **token_struct_list, t_env *env_head, int *exit_status)
{
	t_quote *quoted_list;
	int 	i;
	int		j;
	int		count;
	char	*temp_str;

	count = 0;
	i = 0;
	while (token_struct_list[i])
	{
		if (token_struct_list[i]->type == COMMAND)
			count++;
		i++;
	}
	quoted_list = (t_quote *)ft_calloc(count + 1, sizeof(t_quote));
	if (!quoted_list)
	{
		perror("Malloc failed");
		ft_free_struct_list(token_struct_list);
		return (1);
	}
	quoted_list[count].value = 0;
	i = 0;
	j = 0;
	while (token_struct_list[i])
	{
		if (token_struct_list[i]->type == COMMAND)
		{
			quoted_list[j].index = i;
			quoted_list[j].value = ft_split(token_struct_list[i]->value);
			if (!quoted_list[j].value)
			{
				perror("Malloc failed");
				ft_free_struct_list(token_struct_list);
				i = 0;
				while (quoted_list[i].value)
				{
					ft_free_list(quoted_list[i].value);
					i++;
				}
				free(quoted_list);
				return (1);
			}
			j++;
		}
		i++;
	}

	i = 0;
	j = 0;
	while (quoted_list[i].value)
	{
		j = 0;
		while (quoted_list[i].value[j])
		{
			if (quoted_list[i].value[j][0] == '\'')//strlen 1 e esitse ve tirnak ise hata ver
			{
				if (quoted_list[i].value[j][ft_strlen(quoted_list[i].value[j]) - 1] != '\'')
				{
					ft_putstr_fd("Error: unclosed quote\n", STDERR_FILENO);
					ft_free_struct_list(token_struct_list);
					i = 0;
					while (quoted_list[i].value)
					{
						ft_free_list(quoted_list[i].value);
						i++;
					}
					free(quoted_list);
					return (1);
				}
				temp_str = quoted_list[i].value[j];
				quoted_list[i].value[j] = ft_strtrim(quoted_list[i].value[j], "\'");
				if (!quoted_list[i].value[j])
				{
					perror("Malloc failed");
					ft_free_struct_list(token_struct_list);
					i = 0;
					while (quoted_list[i].value)
					{
						ft_free_list(quoted_list[i].value);
						i++;
					}
					free(quoted_list);
					return (1);
				}
				free(temp_str);
			}
			else
			{
				if (quoted_list[i].value[j][0] == '\"')
				{
					if (quoted_list[i].value[j][ft_strlen(quoted_list[i].value[j]) - 1] != '\"')
					{
						ft_putstr_fd("Error: unclosed quote\n", STDERR_FILENO);
						ft_free_struct_list(token_struct_list);
						i = 0;
						while (quoted_list[i].value)
						{
							ft_free_list(quoted_list[i].value);
							i++;
						}
						free(quoted_list);
						return (1);
					}
					temp_str = quoted_list[i].value[j];
					quoted_list[i].value[j] = ft_strtrim(quoted_list[i].value[j], "\"");
					if (!quoted_list[i].value[j])
					{
						perror("Malloc failed");
						ft_free_struct_list(token_struct_list);
						i = 0;
						while (quoted_list[i].value)
						{
							ft_free_list(quoted_list[i].value);
							i++;
						}
						free(quoted_list);
						return (1);
					}
					free(temp_str);
				}
				temp_str = quoted_list[i].value[j];
				quoted_list[i].value[j] = ft_dollar_sign_expansion(quoted_list[i].value[j], env_head, exit_status);
				if (!quoted_list[i].value[j])
					{
						perror("Malloc failed");
						ft_free_struct_list(token_struct_list);
						i = 0;
						while (quoted_list[i].value)
						{
							ft_free_list(quoted_list[i].value);
							i++;
						}
						free(quoted_list);
						return (1);
					}
					free(temp_str);
			}
			j++;
		}
		i++;
	}

	i = 0;
	j = 0;
	while (quoted_list[i].value)
	{
		j = 0;
		while (quoted_list[i].value[j])
		{
			if (j == 0)
			{
				temp_str = token_struct_list[quoted_list[i].index]->value;
				token_struct_list[quoted_list[i].index]->value = ft_strdup(quoted_list[i].value[j]);
				if (!token_struct_list[quoted_list[i].index]->value)
				{
					perror("Malloc failed");
					free(temp_str);
					ft_free_struct_list(token_struct_list);
					i = 0;
					while (quoted_list[i].value)
					{
						ft_free_list(quoted_list[i].value);
						i++;
					}
					free(quoted_list);
					return (1);
				}
				free(temp_str);
			}
			else
			{
				temp_str = token_struct_list[quoted_list[i].index]->value;
				token_struct_list[quoted_list[i].index]->value = ft_strjoin(token_struct_list[quoted_list[i].index]->value, quoted_list[i].value[j]);
				if (!token_struct_list[quoted_list[i].index]->value)
				{
					perror("Malloc failed");
					free(temp_str);
					ft_free_struct_list(token_struct_list);
					i = 0;
					while (quoted_list[i].value)
					{
						ft_free_list(quoted_list[i].value);
						i++;
					}
					free(quoted_list);
					return (1);
				}
				free(temp_str);
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (quoted_list[i].value)
	{
		ft_free_list(quoted_list[i].value);
		i++;
	}
	free(quoted_list);

	return (0);
}

int	ft_prompt_hook(char *prompt, t_env *env_head, int *exit_status)
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
		return (1);
	}
	if (ft_quote_expansion(token_struct_list, env_head, exit_status))
		return (1);
	//test
	int i = 0;
	while (token_struct_list[i])
	{
		printf("kind= %d token_struct_list[%d]->value = %s\n", token_struct_list[i]->type, i, token_struct_list[i]->value);
		i++;
	}
	//test
	ft_free_struct_list(token_struct_list);
	return (0);
}

void	ft_env_lstadd_back(t_env **env, t_env *new)
{
	t_env	*temp;

	if (!*env)
	{
		*env = new;
		return ;
	}
	temp = *env;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

void	ft_env_lstadd_front(t_env **env, t_env *new)
{
	if (!*env)
	{
		*env = new;
		return ;
	}
	new->next = *env;
	*env = new;
}

void	ft_env_lstclear(t_env **env)
{
	t_env	*temp;

	if (!*env)
		return ;
	while (*env)
	{
		temp = *env;
		*env = (*env)->next;
		free(temp->var_name);
		free(temp->var_value);
		free(temp);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	int		status;
	char	*prompt;
	t_env	*env;
	t_env	*env_head;

	int	i;

	i = 0;
	while (envp[i])
	{
		env = (t_env *)ft_calloc(1 ,sizeof(t_env));
		if (!env)
		{
			perror("Malloc failed");
			ft_env_lstclear(&env_head);
			return (1);
		}
		env->var_name = ft_substr(envp[i], 0, ft_strchr(envp[i], '=') - envp[i]);
		if (!env->var_name)
		{
			perror("Malloc failed");
			ft_env_lstclear(&env_head);
			free(env);
			return (1);
		}
		env->var_value = ft_substr(envp[i], ft_strchr(envp[i], '=') - envp[i] + 1, ft_strlen(envp[i]));
		if (!env->var_value)
		{
			perror("Malloc failed");
			ft_env_lstclear(&env_head);
			free(env->var_name);
			free(env);
			return (1);
		}
		env->next = NULL;
		if (!i)
			env_head = env;
		else
			ft_env_lstadd_back(&env_head, env);
		i++;
	}
//##############silinece##############
	t_env *iter;
	iter = env_head;
	while (iter)
	{//tek satirda argumani yaz
		printf("%s=", iter->var_name);
		printf("%s\n", iter->var_value);
		iter = iter->next;
	}
//##############silinece##############


	status = 0;
	while (1)
	{
		prompt = readline("minishell$ ");
		if (!prompt)
			break ;
		add_history(prompt);
		ft_prompt_hook(prompt, env_head, &status);
	}
	ft_env_lstclear(&env_head);
	return (status);
}
