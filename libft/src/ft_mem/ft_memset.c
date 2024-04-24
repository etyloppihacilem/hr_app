/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 12:20:24 by nserve            #+#    #+#             */
/*   Updated: 2022/11/23 11:31:41 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*s_byte;
	unsigned char	c_byte;
	size_t			i;

	if (!s)
		return (NULL);
	s_byte = s;
	c_byte = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		s_byte[i] = c_byte;
		i++;
	}
	return ((void *)s_byte);
}
