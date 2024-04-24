/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 12:22:05 by nserve            #+#    #+#             */
/*   Updated: 2022/11/24 11:12:47 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	ret;

	ret = ft_strlen(src);
	if (!src)
		return (0);
	if (size == 0)
		return (ret);
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
