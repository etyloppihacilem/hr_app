/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 12:21:17 by nserve            #+#    #+#             */
/*   Updated: 2023/07/13 15:12:44 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_extract(char **tab, const char *s, char c, size_t tabsize);
static size_t	ft_tabsize(const char *s, char c);
static size_t	ft_substrlen(const char *s, char c);
static void		ft_freetab(char **tab, size_t len);

char	**ft_split(const char *s, char c)
{
	char	**tab;
	size_t	tabsize;

	tabsize = ft_tabsize(s, c);
	tab = (char **)malloc(sizeof(char *) * (tabsize + 1));
	if (!tab || !s)
		return (NULL);
	tab[tabsize] = NULL;
	if (ft_extract(tab, s, c, tabsize))
		return (tab);
	else
		return (NULL);
}

static int	ft_extract(char **tab, const char *s, char c, size_t tabsize)
{
	size_t	substrsize;
	size_t	i;

	i = 0;
	while (*s && i < tabsize)
	{
		while (*s && *s == c)
			s++;
		if (*s && *s != c)
		{
			substrsize = ft_substrlen(s, c);
			tab[i] = ft_substr(s, 0, substrsize);
			if (!tab[i])
			{
				ft_freetab(tab, i - 1);
				return (0);
			}
			i++;
			s += substrsize;
		}
	}
	return (1);
}

static size_t	ft_tabsize(const char *s, char c)
{
	size_t	size;

	size = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s && *s != c)
			size++;
		while (*s && *s != c)
			s++;
	}
	return (size);
}

static size_t	ft_substrlen(const char *s, char c)
{
	size_t	size;

	if (!s)
		return (0);
	size = 0;
	while (s[size] != c && s[size])
		size++;
	return (size);
}

static void	ft_freetab(char **tab, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return ;
}
