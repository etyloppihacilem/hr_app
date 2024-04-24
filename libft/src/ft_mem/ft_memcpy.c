/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 12:20:14 by nserve            #+#    #+#             */
/*   Updated: 2022/11/24 11:30:40 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*dst_byte;
	const unsigned char	*src_byte;
	size_t				i;

	if (!dst)
		return (NULL);
	dst_byte = dst;
	src_byte = src;
	i = 0;
	while (i < n)
	{
		dst_byte[i] = src_byte[i];
		i++;
	}
	return ((void *)dst_byte);
}
