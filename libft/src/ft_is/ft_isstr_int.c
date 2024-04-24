/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isstr_int.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 10:54:00 by nserve            #+#    #+#             */
/*   Updated: 2023/02/22 11:19:41 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isstr_int(char *str)
{
	int		size;
	char	*int_min;
	char	*int_max;

	if (!str)
		return (0);
	size = ft_strlen(str);
	if (size > 10 || (str[0] == '-' && size > 11))
		return (0);
	else if (size == 10)
	{
		int_max = ft_itoa(INT_MAX);
		if (ft_strncmp(int_max, str, 10) < 0)
			return (free(int_max), 0);
		free(int_max);
	}
	else if (size == 11 && str[0] == '-')
	{
		int_min = ft_itoa(INT_MIN);
		if (ft_strncmp(int_min, str, 11) < 0)
			return (free(int_min), 0);
		free(int_min);
	}
	return (1);
}
