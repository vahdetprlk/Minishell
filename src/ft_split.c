#include "minishell.h"
#include <stdlib.h>

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

static int	ft_word_counter(char const *s)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (s[i] == '\0')
		return (0);
	while (s[i] != '\0')
	{
		if (s[i] == '\'')
		{
			i++;
			count++;
			while (s[i] && s[i] != '\'')
				i++;
			if (s[i] == '\'')
				i++;
		}
		else if (s[i] == '\"')
		{
			i++;
			count++;
			while (s[i] && s[i] != '\"')
				i++;
			if (s[i] == '\"')
				i++;
		}
		else if (!ft_strncmp(&s[i], "<<", 2) || !ft_strncmp(&s[i], ">>", 2)
				|| !ft_strncmp(&s[i], "||", 2) || !ft_strncmp(&s[i], "&&", 2))
		{
			count++;
			i += 2;
		}
		else if (s[i] == '>' || s[i] == '<' || s[i] == '|')
		{
			count++;
			i++;
		}
		else if (s[i] == ' ')
			i++;
		else
		{
			count++;
			while (s[i] && s[i] != ' ' && s[i] != '\'' && s[i] != '\"')
				i++;
		}
	}
	return (count);
}

static int	ft_wordlen(char const *s, char c)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	if (s[i] == '\0')
		return (0);
	if (c == '\'')
	{
		while (s[i] != c &&  s[i] != '\0')
		{
			len++;
			i++;
		}
		return (len);
	}
	else if (c == '\"')
	{
		while (s[i] != c &&  s[i] != '\0')
		{
			len++;
			i++;
		}
		return (len);
	}
	else
		while (ft_strncmp(&s[i], "<<", 2) && ft_strncmp(&s[i], ">>", 2)
			&& ft_strncmp(&s[i], "||", 2) && ft_strncmp(&s[i], "&&", 2)
			&& s[i] != ' ' && s[i] != '\'' && s[i] != '\"' && s[i] != '>'
			&& s[i] != '<' && s[i] != '|' && s[i] != '\0')
		{
			len++;
			i++;
		}
	return (len);
}

static char	**ft_free(char **str, int i)
{
	while (i >= 0)
	{
		free(str[i]);
		i--;
	}
	free(str);
	return (NULL);
}

char	**ft_split(char const *s)
{
	char	**str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s)
		return (NULL);
	str = (char **)ft_calloc((ft_word_counter(s) + 1), sizeof(char *));
	if (!str)
		return (NULL);
	while (i < ft_word_counter(s))
	{
		while (s[j] == ' ')
			j++;
		if (!ft_strncmp(&s[j], "<<", 2) || !ft_strncmp(&s[j], ">>", 2) || !ft_strncmp(&s[j], "||", 2) || !ft_strncmp(&s[j], "&&", 2))
		{
			str[i] = ft_substr(s, j, 2);
			if (!str[i])
				return (ft_free(str, i));
			j = j + 2;
		}
		else if (s[j] == '>' || s[j] == '<' || s[j] == '|')
		{
			j++;
			str[i] = ft_substr(s, j - 1, 1);
			if (!str[i])
				return (ft_free(str, i));
		}
		else if (s[j] == '\'')
		{
			j++;
			str[i] = ft_substr(s, j - 1, ft_wordlen(&s[j], '\'') + 2);
			if (!str[i])
				return (ft_free(str, i));
			j = j + ft_wordlen(&s[j], '\'');
			if (s[j] == '\'')
				j++;
		}
		else if (s[j] == '\"')
		{
			j++;
			str[i] = ft_substr(s, j - 1, ft_wordlen(&s[j], '\"') + 2);
			if (!str[i])
				return (ft_free(str, i));
			j = j + ft_wordlen(&s[j], '\"');
			if (s[j] == '\"')
				j++;
		}
		else
		{
			str[i] = ft_substr(s, j, ft_wordlen(&s[j], ' '));
			if (!str[i])
				return (ft_free(str, i));
			j = j + ft_wordlen(&s[j], ' ');
		}
		i++;
	}
	str[i] = NULL;
	return (str);
}
