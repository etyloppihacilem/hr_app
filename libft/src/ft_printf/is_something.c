/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_something.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmelica <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 10:57:23 by hmelica           #+#    #+#             */
/*   Updated: 2023/10/01 14:31:03 by hmelica          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Return 1 if c is a valid flag
*/
int	is_flag(char c)
{
	int	i;

	i = 0;
	while (i++ < 5)
		if (c == "-0# +"[i - 1])
			return (1);
	return (0);
}

/*
Return 1 if c is a valid conversion specifier
*/
int	is_csp(char c)
{
	int	i;

	i = 0;
	while (i++ < 9)
		if (c == "cspdiuxX%"[i - 1])
			return (1);
	return (0);
}

/*
Read the following number, and stop on the next non digit char
*/
int	read_following_int(const char **format)
{
	int	ret;
	int	signe;

	ret = 0;
	signe = 1;
	if (format && *format && **format == '-')
	{
		signe = -1;
		(*format)++;
	}
	while (format && *format && **format && ft_isdigit(**format))
	{
		ret *= 10;
		ret += (**format - '0') * signe;
		(*format)++;
	}
	return (ret);
}
