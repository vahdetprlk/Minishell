#include "minishell.h"

char	*ft_strdup(const char *s1)
{
	size_t	len_s1;
	char	*dst;

	len_s1 = ft_strlen(s1) + 1;
	dst = (char *)ft_calloc(len_s1, sizeof(char));
	if (dst == NULL)
		return (dst);
	ft_strlcpy(dst, s1, len_s1);
	return (dst);
}
