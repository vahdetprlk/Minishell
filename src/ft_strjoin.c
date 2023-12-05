#include "minishell.h"

static size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	len_src;
	size_t	len_dst;

	len_src = ft_strlen(src);
	len_dst = ft_strlen(dst);
	if (dstsize <= len_dst)
		return (dstsize + len_src);
	dst += len_dst;
	ft_strlcpy(dst, src, (dstsize - len_dst));
	return (len_src + len_dst);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*dst;
	size_t	dstsize;

	dstsize = ft_strlen(s1) + ft_strlen(s2) + 1;
	dst = (char *)ft_calloc(dstsize, sizeof(char));
	if (dst == NULL)
		return (NULL);
	ft_strlcpy(dst, s1, dstsize);
	ft_strlcat(dst, s2, dstsize);
	return (dst);
}
