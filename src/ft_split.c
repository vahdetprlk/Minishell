#include "minishell.h"

static size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

static size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	k;

	i = 0;
	while (src[i])
		i++;
	if (dstsize == 0)
		return (i);
	k = 0;
	while (src[k] != '\0' && k < dstsize - 1)
	{
		dst[k] = src[k];
		k++;
	}
	dst[k] = '\0';
	return (i);
}

static char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*dst;
	size_t	len_str;

	len_str = ft_strlen(s);
	if (start >= len_str)
	{
		dst = (char *)ft_calloc(1, sizeof(char));
		if (!dst)
			return (NULL);
		*dst = '\0';
	}
	else
	{
		if ((len_str - start) < len)
			len = len_str - start;
		if (len_str >= len)
			dst = (char *)ft_calloc(len + 1 ,sizeof(char));
		else
			dst = (char *)ft_calloc(len_str + 1 ,sizeof(char));
		if (dst == NULL)
			return (NULL);
		ft_strlcpy(dst, s + start, len + 1);
	}
	return (dst);
}

static size_t	word_counter(char const *s, char c)
{
	size_t	word_counter;

	word_counter = 0;
	while (*s)
	{
		if (*s == c)
			s++;
		else
		{
			word_counter++;
			while (*s && *s != c)
				s++;
		}
	}
	return (word_counter);
}

char	**ft_split(char const *s, char c)
{
	char	**dst;
	size_t	word_size;
	size_t	letter_counter;

	word_size = word_counter(s, c);
	dst = (char **)ft_calloc((word_size + 1), sizeof(char *));
	if (!dst)
		return (NULL);
	while (*s)
	{
		if (*s == c)
			s++;
		else
		{
			letter_counter = 0;
			while (*s && *s != c && ++letter_counter)
				s++;
			*dst++ = ft_substr(s - letter_counter, 0, letter_counter);
		}
	}
	*dst = 0;
	return (dst - word_size);
}
