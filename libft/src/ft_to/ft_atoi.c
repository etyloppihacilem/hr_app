/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 12:14:57 by nserve            #+#    #+#             */
/*   Updated: 2022/11/24 11:04:14 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	sign;
	int	nbr;

	if (!str)
		return (0);
	sign = 1;
	nbr = 0;
	while (*str == ' ' || ('\t' <= *str && *str <= '\r'))
		str++;
	if ((*str == '+' || *str == '-'))
	{
		if (*str == '-')
			sign = -1 * sign;
		str++;
	}
	while ('0' <= *str && *str <= '9')
	{
		nbr = 10 * nbr + (*str - '0');
		str++;
	}
	return (sign * nbr);
}
