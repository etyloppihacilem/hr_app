/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 12:27:35 by nserve            #+#    #+#             */
/*   Updated: 2022/11/24 11:15:59 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_trimlen(const char *s, const char *set);

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*ret;
	size_t	trimsize;

	if (!s1 || !set)
		return (NULL);
	trimsize = ft_trimlen(s1, set);
	ret = malloc(sizeof(*ret) * (trimsize + 1));
	if (!ret)
		return (NULL);
	ret[trimsize] = '\0';
	while (*s1 && ft_strchr(set, (int)*s1))
		s1++;
	if (!*s1)
		return (ret);
	ft_memcpy(ret, s1, trimsize);
	return (ret);
}

static size_t	ft_trimlen(const char *s, const char *set)
{
	size_t	size;

	while (*s && ft_strchr(set, (int)*s))
		s++;
	if (!*s)
		return (0);
	size = ft_strlen(s);
	while (ft_strchr(set, (int)s[size - 1]))
	{
		size--;
	}
	return (size);
}
