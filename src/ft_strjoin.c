#include "minishell.h"
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*dst;
	size_t	dstsize;

	dstsize = ft_strlen(s1) + ft_strlen(s2) + 1;
	dst = (char *)ft_calloc(dstsize ,sizeof(char));
	if (dst == NULL)
		return (NULL);
	ft_strlcpy(dst, s1, dstsize);
	free((char *)s1);
	ft_strlcat(dst, s2, dstsize);
	return (dst);
}
