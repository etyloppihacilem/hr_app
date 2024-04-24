/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 12:22:31 by nserve            #+#    #+#             */
/*   Updated: 2022/11/24 11:15:26 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	unsigned char	c_byte;
	size_t			size;

	if (!s)
		return (NULL);
	c_byte = (unsigned char)c;
	size = ft_strlen(s);
	if (c_byte == '\0')
		return ((char *)(s + size));
	while (size)
	{
		if ((unsigned char)s[size] == c_byte)
			return ((char *)(s + size));
		size--;
	}
	if ((unsigned char)s[0] == c_byte)
		return ((char *)s);
	else
		return (NULL);
}
