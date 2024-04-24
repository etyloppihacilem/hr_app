/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmelica <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 14:06:03 by hmelica           #+#    #+#             */
/*   Updated: 2023/04/21 15:44:35 by hmelica          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
FLAGS : every binary 1 tells that the flag exists
1   '-'
2   '0'
4   '#'
8   ' '
16  '+'

return the decimal value in a flag (binary 1)
*/
int	set_flag(char c)
{
	if (c == '-')
		return (1);
	if (c == '0')
		return (2);
	if (c == '#')
		return (4);
	if (c == ' ')
		return (8);
	if (c == '+')
		return (16);
	return (0);
}

/*
return the closest flag to a nb
*/
static char	get_flag(int nb)
{
	if (nb >= 16)
		return ('+');
	if (nb >= 8)
		return (' ');
	if (nb >= 4)
		return ('#');
	if (nb >= 2)
		return ('0');
	if (nb >= 1)
		return ('-');
	return ('\0');
}

/*
check if a flag is in a number (if the binary is 1)
*/
int	check_flag(int nb, char c)
{
	int	i;

	i = 16;
	while (i > 0)
	{
		if (nb >= i)
		{
			if (get_flag(nb) == c)
				return (1);
			nb -= i;
		}
		i /= 2;
	}
	return (0);
}

/*
- override 0
+ override ' '

handle flag overriding by deleting them
*/
void	compile_flags(t_insert *ins)
{
	if ((check_flag(ins->flags, '-') && check_flag(ins->flags, '0'))
		|| (ins->prec >= 0 && check_flag(ins->flags, '0')))
		ins->flags -= set_flag('0');
	if (check_flag(ins->flags, '+') && check_flag(ins->flags, ' '))
		ins->flags -= set_flag(' ');
}
