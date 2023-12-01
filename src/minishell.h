#ifndef MINISHELL_H
# define MINISHELL_H

# include <stddef.h>

typedef enum e_token_type {
	COMMAND,
	HEREDOC,
	APPEND,
	PIPE,
	INPUT_REDIRECT,
	OUTPUT_REDIRECT
}	t_token_type;

typedef struct s_token {
	char			*value;
	t_token_type	type;
}	t_token;

void	*ft_calloc(size_t count, size_t size);
char	**ft_tokenization(char const *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	ft_putstr_fd(char *s, int fd);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
char	*ft_strdup(const char *s1);
size_t	ft_strlen(const char *s);

char	**ft_split(char const *s);

#endif
