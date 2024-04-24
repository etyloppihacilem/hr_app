/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hexa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmelica <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 18:39:28 by hmelica           #+#    #+#             */
/*   Updated: 2023/11/09 14:11:29 by hmelica          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	hex_rec(unsigned int i, char *tab, int fd)
{
	if (i / 16 && hex_rec(i / 16, tab, fd) < 0)
		return (-1);
	if (write(fd, tab + (i % 16), 1) < 0)
		return (-1);
	return (1);
}

static int	write_x_number(unsigned int i, t_insert ins, char *tab, int fd)
{
	char	c;
	int		ret;

	ret = 0;
	c = ' ';
	if (i != 0 && check_flag(ins.flags, '#'))
	{
		ret += 2;
		if (write(fd, "0", 1) < 0 || write(fd, tab + 16, 1) < 0)
			return (-1);
	}
	if (check_flag(ins.flags, '0'))
		c = '0';
	if (ins.min_width > 0 && !check_flag(ins.flags, '-'))
	{
		while (ins.min_width-- > 0 && ++ret)
			if (write(fd, &c, 1) < 0)
				return (-1);
	}
	while (ins.prec-- > 0 || (ins.prec++ > 0))
		if (write(fd, "0", 1) < 0)
			return (-1);
	if (ins.null_prec && i == 0)
		return (ret);
	return (hex_rec(i, tab, fd) * ret);
}

int	hex_len(unsigned long i)
{
	int		ret;

	ret = 1;
	if (i / 16)
		ret += hex_len(i / 16);
	return (ret);
}

int	main_x(va_list act, t_insert ins, char *tab, int fd)
{
	int				ret;
	int				tmp;
	unsigned int	i;

	ret = 0;
	i = va_arg(act, unsigned int);
	ins.null_prec = 0;
	if (ins.prec == 0)
		ins.null_prec = 1;
	ins.prec = (ins.prec - hex_len(i)) * !check_flag(ins.flags, '0');
	ret += hex_len(i) + (ins.prec * (ins.prec > 0)) - (ins.null_prec && i == 0);
	ins.min_width -= ret;
	tmp = write_x_number(i, ins, tab, fd);
	if (tmp < 0)
		return (-1);
	ret += tmp;
	if (ins.min_width > 0 && check_flag(ins.flags, '-'))
	{
		while (ins.min_width-- > 0 && ++ret)
			if (write(fd, " ", 1) < 0)
				return (-1);
	}
	return (ret);
}
