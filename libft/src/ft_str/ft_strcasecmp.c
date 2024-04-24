/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcasecmp.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmelica <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 14:26:05 by hmelica           #+#    #+#             */
/*   Updated: 2023/10/28 15:42:40 by hmelica          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

int	ft_strcasecmp(const char *s1, const char *s2)
{
	int	is_same;

	if (!s1 || !s2)
		return (0);
	while (*s1 && *s2 && (unsigned char)*s1 == (unsigned char)*s2)
	{
		s1++;
		while (*s1 && !ft_isalnum(*s1))
			s1++;
		s2++;
		while (*s2 && !ft_isalnum(*s2))
			s2++;
	}
	is_same = (ft_tolower(*s1) == ft_tolower(*s2)) * 1024;
	return ((ft_tolower(*s1) + (ft_islower(*s1) * is_same))
		- (ft_tolower(*s2) + (ft_islower(*s2) * is_same)));
}
