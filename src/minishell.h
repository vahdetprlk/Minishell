#ifndef MINISHELL_H
# define MINISHELL_H

# include <stddef.h>

void	*ft_calloc(size_t count, size_t size);
char	**ft_tokenization(char const *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

#endif
