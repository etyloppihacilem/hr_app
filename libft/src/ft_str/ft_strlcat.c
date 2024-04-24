/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 12:21:57 by nserve            #+#    #+#             */
/*   Updated: 2022/11/24 11:12:35 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dstsize;
	size_t	ret;

	if (!dst || !src)
		return (0);
	dstsize = 0;
	while (*dst)
	{
		dst++;
		dstsize++;
	}
	if (size == 0 || size < dstsize)
		return (size + ft_strlen(src));
	size = size - dstsize;
	ret = dstsize + ft_strlen(src);
	while (*src && size)
	{
		*dst = *src;
		src++;
		size--;
		if (size)
			dst++;
	}
	*dst = '\0';
	return (ret);
}
