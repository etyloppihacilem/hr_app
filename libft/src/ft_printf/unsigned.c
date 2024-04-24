/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsigned.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmelica <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 18:39:28 by hmelica           #+#    #+#             */
/*   Updated: 2023/11/03 15:53:50 by hmelica          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	write_u_number(unsigned int i, t_insert ins, int fd)
{
	char	c;
	int		ret;

	ret = 0;
	c = ' ';
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
	return (dec_rec(i, fd) * ret);
}

int	main_u(va_list act, t_insert ins, int fd)
{
	int				ret;
	int				tmp;
	unsigned int	i;

	ret = 0;
	i = va_arg(act, unsigned int);
	ins.null_prec = 0;
	if (ins.prec == 0)
		ins.null_prec = 1;
	ins.prec = (ins.prec - int_len(i)) * !check_flag(ins.flags, '0');
	ret += int_len(i) + (ins.prec * (ins.prec > 0)) - (ins.null_prec && i == 0);
	ins.min_width -= ret;
	tmp = write_u_number(i, ins, fd);
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
