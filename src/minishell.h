#ifndef MINISHELL_H
# define MINISHELL_H

# include <stddef.h>

typedef struct s_file
{
	int		fd;
	char	*name;
}	t_file;

void	*ft_calloc(size_t count, size_t size);
char	**ft_split(char const *s, char c);

#endif
