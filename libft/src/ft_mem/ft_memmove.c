/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 12:20:18 by nserve            #+#    #+#             */
/*   Updated: 2022/11/24 11:09:44 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	unsigned char		*dst_byte;
	const unsigned char	*src_byte;
	size_t				i;

	if (!dst || !src)
		return (NULL);
	dst_byte = dst;
	src_byte = src;
	if (dst_byte > src_byte && dst_byte < n + src_byte)
	{
		i = 0;
		while (i < n)
		{
			dst_byte[n - 1] = src_byte[n - 1];
			n--;
		}
		return ((void *)dst_byte);
	}
	else
		return (ft_memcpy(dst, src, n));
}
