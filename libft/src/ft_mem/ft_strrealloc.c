/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrealloc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 09:55:26 by nserve            #+#    #+#             */
/*   Updated: 2023/10/28 09:55:41 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

char	*ft_strrealloc(char *src, int new_len)
{
	char	*ret;
	int		x;

	ret = malloc(sizeof(char) * new_len);
	if (!ret)
		return (NULL);
	ft_memset(ret, 0, sizeof(char) * new_len);
	x = 0;
	while (src[x])
	{
		ret[x] = src[x];
		x++;
	}
	return (ret);
}
