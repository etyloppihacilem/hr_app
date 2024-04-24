/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 12:22:26 by nserve            #+#    #+#             */
/*   Updated: 2022/11/24 16:10:16 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

char	*ft_strnstr(const char *s, const char *to_find, size_t len)
{
	size_t	to_findsize;
	size_t	i;

	if (!s || !to_find)
		return (NULL);
	if (!(*to_find))
		return ((char *)s);
	to_findsize = ft_strlen(to_find);
	while (*s && len)
	{
		i = 0;
		while (s[i] == to_find[i] && to_find[i] && len >= to_findsize)
		{
			i++;
		}
		if (to_find[i] == '\0')
			return ((char *)s);
		s++;
		len--;
	}
	return (NULL);
}
