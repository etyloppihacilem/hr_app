/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 12:20:08 by nserve            #+#    #+#             */
/*   Updated: 2022/11/24 11:08:05 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*s1_byte;
	const unsigned char	*s2_byte;
	size_t				i;

	if (!s1 || !s2)
		return (0);
	s1_byte = s1;
	s2_byte = s2;
	i = 0;
	while (i < n)
	{
		if (s1_byte[i] != s2_byte[i])
		{
			return (s1_byte[i] - s2_byte[i]);
		}
		i++;
	}
	return (0);
}
