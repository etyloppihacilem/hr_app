/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 12:21:25 by nserve            #+#    #+#             */
/*   Updated: 2022/11/24 11:11:28 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char	c_byte;

	if (!s)
		return (NULL);
	c_byte = (unsigned char)c;
	while (*s)
	{
		if ((unsigned char)*s == c_byte)
			return ((char *)s);
		s++;
	}
	if (c_byte == '\0')
		return ((char *)s);
	else
		return (NULL);
}
