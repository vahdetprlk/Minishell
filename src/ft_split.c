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
			dst = (char *)ft_calloc(len + 1, sizeof(char));
		else
			dst = (char *)ft_calloc(len_str + 1, sizeof(char));
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
		if (!ft_strncmp(&s[i], "<<", 2) || !ft_strncmp(&s[i], ">>", 2)
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
			while (s[i] && s[i] == ' ')
				i++;
		else
		{
			count++;
			while (s[i] && s[i] != ' ' && s[i] != '>'
				&& s[i] != '<' && s[i] != '|')
			{
				if (s[i] == '\'')
				{
					i++;
					while (s[i] && s[i] != '\'')
						i++;
					if (s[i] == '\'')
						i++;
				}
				else if (s[i] == '\"')
				{
					i++;
					while (s[i] && s[i] != '\"')
						i++;
					if (s[i] == '\"')
						i++;
				}
				else
					i++;
			}
		}
	}
	return (count);
}

static int	ft_wordlen(char const *s)
{
	int	i;
	int	len;
	int	in_quote_d;
	int	in_quote_s;

	i = 0;
	in_quote_d = 0;
	in_quote_s = 0;
	len = 0;
	while (s[i] != '\0')
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			if (s[i] == '\'')
				in_quote_s = !in_quote_s;
			else
				in_quote_d = !in_quote_d;
			len++;
			i++;
		}
		else if (in_quote_d == 0 && in_quote_s == 0
			&& (!ft_strncmp(&s[i], "<<", 2) || !ft_strncmp(&s[i], ">>", 2)
				|| !ft_strncmp(&s[i], "||", 2) || !ft_strncmp(&s[i], "&&", 2)
				|| s[i] == ' ' || s[i] == '>' || s[i] == '<' || s[i] == '|'))
			break ;
		else
		{
			len++;
			i++;
		}
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

char	**ft_tokenization(char const *s)
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
		if (!ft_strncmp(&s[j], "<<", 2) || !ft_strncmp(&s[j], ">>", 2)
			|| !ft_strncmp(&s[j], "||", 2) || !ft_strncmp(&s[j], "&&", 2))
		{
			str[i] = ft_substr(s, j, 2);
			if (!str[i])
				return (ft_free(str, i));
			j = j + 2;
		}
		else if (s[j] == '>' || s[j] == '<' || s[j] == '|')
		{
			str[i] = ft_substr(s, j, 1);
			if (!str[i])
				return (ft_free(str, i));
			j++;
		}
		else if (s[j] == ' ')
		{
			while (s[j] && s[j] == ' ')
				j++;
			continue ;
		}
		else
		{
			str[i] = ft_substr(s, j, ft_wordlen(&s[j]));
			if (!str[i])
				return (ft_free(str, i));
			j = j + ft_wordlen(&s[j]);
		}
		i++;
	}
	str[i] = NULL;
	return (str);
}
