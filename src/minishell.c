#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int ft_word_counter(char* prompt)
{
	int i = 0;
	int count = 0;
	int length = strlen(prompt);
	int in_quotes = 0;

	while(i < length) {
		if(prompt[i] == '\"' || prompt[i] == '\'') {
			in_quotes = !in_quotes;
		}
		if((prompt[i] == ' ' && !in_quotes && prompt[i+1] != ' ' && prompt[i+1] != '\0') || 
		   (prompt[i] == '\"' || prompt[i] == '\'')) {
			count++;
		}
		if((prompt[i] == '|' || prompt[i] == '<' || prompt[i] == '>') && !in_quotes) {
			count++;
		}
		i++;
	}

	if(prompt[length - 1] != ' ' && !(prompt[length - 1] == '|' && (prompt[length - 2] == ' ' || length == 1))) {
		count++;
	}

	return count;
}

char** split_prompt(char* prompt) {
    int length = strlen(prompt);
    char** words = malloc((length + 1) * sizeof(char*));
    for(int i = 0; i <= length; i++) {
        words[i] = malloc((length + 1) * sizeof(char));
    }

    int i = 0, j = 0, k = 0;
    int in_quotes = 0;
    while(prompt[i]) {
        if(prompt[i] == '\"' || prompt[i] == '\'') {
            words[j][k] = prompt[i];
            k++;
            if(in_quotes && k != 0) {
                words[j][k] = '\0';
                j++;
                k = 0;
            }
            in_quotes = !in_quotes;
        } else if((prompt[i] == '|' || prompt[i] == '<' || prompt[i] == '>') && !in_quotes) {
            if(k != 0) {
                words[j][k] = '\0';
                j++;
                k = 0;
            }
            words[j][0] = prompt[i];
            words[j][1] = '\0';
            j++;
        } else if((prompt[i] != ' ' || in_quotes) && prompt[i] != '\0') {
            words[j][k] = prompt[i];
            k++;
        } else if(prompt[i] == ' ' && !in_quotes) {
            if(k != 0) {
                words[j][k] = '\0';
                j++;
                k = 0;
            }
        }
        i++;
    }
    if(k != 0) {
        words[j][k] = '\0';
        j++;
    }
    words[j] = NULL;

    return words;
}

void	ft_prompt_hook(char *prompt)
{
	ft_word_counter(prompt);
	printf("word count: %d\n", ft_word_counter(prompt));
	char** words = split_prompt(prompt);
	for(int i = 0; words[i]; i++) {
		printf("cmd[%d] %s\n",i, words[i]);
	}
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
