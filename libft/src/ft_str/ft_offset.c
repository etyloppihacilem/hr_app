/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_offset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmelica <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 19:35:21 by hmelica           #+#    #+#             */
/*   Updated: 2023/10/25 20:12:40 by hmelica          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

/*
 * décale de len à partir d'offset
 * */
void	ft_offset(char *s, size_t off)
{
	size_t	len;
	size_t	i;

	if (!s || off == 0)
		return ;
	len = ft_strlen(s);
	i = 0;
	while (i < len)
	{
		if (i + off < len)
			s[i] = s[i + off];
		else
			s[i] = '\0';
		i++;
	}
}
