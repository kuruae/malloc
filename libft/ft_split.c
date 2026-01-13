/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:33:36 by emagnani          #+#    #+#             */
/*   Updated: 2024/05/29 10:25:31 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	last_pos(const char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
	{
		i++;
	}
	return (i);
}

static void	ft_free(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free (str[i++]);
	free (str);
}

static int	word_count(char const *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	if (*s == '\0')
		return (0);
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			count++;
		i++;
	}
	return (count);
}

static char	*array(const char *s, char c)
{
	int		i;
	char	*tab;

	i = last_pos(s, c);
	tab = malloc((i + 1) * sizeof(char));
	if (!tab)
	{
		free (tab);
		return (NULL);
	}
	ft_strlcpy(tab, s, i + 1);
	tab[i] = '\0';
	s += i;
	return (tab);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	char	**tab;

	i = 0;
	tab = malloc((word_count(s, c) + 1) * sizeof(char *));
	if (!tab)
		return (NULL);
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (*s)
		{
			tab[i] = array(s, c);
			if (!tab[i++])
			{
				ft_free(tab);
				return (NULL);
			}
			s = s + last_pos(s, c);
		}
	}
	tab[i] = NULL;
	return (tab);
}
