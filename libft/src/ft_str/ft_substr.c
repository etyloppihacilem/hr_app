/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 12:27:28 by nserve            #+#    #+#             */
/*   Updated: 2022/11/24 11:16:18 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

static size_t	ft_sublen(const char *s, size_t len);

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*ret;
	size_t	subsize;
	int		i;

	if (!s)
		return (NULL);
	if (ft_strlen(s) < start)
		return (ft_calloc(sizeof(char), 1));
	s = s + start;
	subsize = ft_sublen(s, len);
	ret = malloc(sizeof(*ret) * (subsize + 1));
	if (!ret)
		return (NULL);
	ret[subsize] = '\0';
	i = 0;
	while (s[i] && len)
	{
		ret[i] = s[i];
		i++;
		len--;
	}
	return (ret);
}

static size_t	ft_sublen(const char *s, size_t len)
{
	size_t	size;

	size = 0;
	while (*s && len)
	{
		size++;
		s++;
		len--;
	}
	return (size);
}
