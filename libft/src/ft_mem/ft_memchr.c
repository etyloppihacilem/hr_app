/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 12:20:04 by nserve            #+#    #+#             */
/*   Updated: 2022/11/24 11:07:28 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*s_byte;
	unsigned char		c_byte;
	size_t				i;

	if (!s)
		return (NULL);
	s_byte = s;
	c_byte = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (s_byte[i] == c_byte)
			return ((void *)(s_byte + i));
		i++;
	}
	return (NULL);
}
