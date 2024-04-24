/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pointer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmelica <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 18:39:28 by hmelica           #+#    #+#             */
/*   Updated: 2023/11/09 14:22:28 by hmelica          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Recursievly write down a pointer
*/
static int	pnt_rec(void *p, int fd)
{
	char	c;
	int		ret;
	int		tmp;

	if (!p)
		return (0);
	ret = 1;
	c = "0123456789abcdef"[(unsigned long)p % 16];
	if ((unsigned long)p / 16)
	{
		tmp = pnt_rec((void *)((unsigned long)p / 16), fd);
		if (tmp < 0)
			return (-1);
		ret += tmp;
	}
	if (write(fd, &c, 1) < 0)
		return (-1);
	return (ret);
}

/*
Check for interesting flags before writing pointer
*/
static int	write_pointer(void *p, t_insert ins, int fd)
{
	int	ret;
	int	tmp;

	ret = 0;
	ins.min_width -= ((2 + hex_len((unsigned long) p)) * (p != 0))
		+ 5 * (p == 0);
	if (ins.min_width > 0 && !check_flag(ins.flags, '-'))
	{
		while (ins.min_width-- > 0 && ++ret)
			if (write(fd, " ", 1) < 0)
				return (-1);
	}
	if ((!p && write(fd, "(nil)", 5) < 0) || (p && write(fd, "0x", 2) < 0))
		return (-1);
	tmp = pnt_rec(p, fd);
	if (tmp < 0)
		return (-1);
	ret += tmp + ((!p * 5));
	if (ins.min_width > 0 && check_flag(ins.flags, '-'))
	{
		while (ins.min_width-- > 0 && ++ret)
			if (write(fd, " ", 1) < 0)
				return (-1);
	}
	return (ret);
}

/*
Writes a pointer argument
*/
int	main_p(va_list act, t_insert ins, int fd)
{
	void	*p;
	int		ret;

	p = va_arg(act, void *);
	if (p && check_flag(ins.flags, ' ') && write(fd, " ", 1) < 0)
		return (-1);
	else if (p && check_flag(ins.flags, '+') && write(fd, "+", 1) < 0)
		return (-1);
	ret = write_pointer(p, ins, fd);
	if (ret < 0)
		return (-1);
	return (2 * (p != 0) + ret + (check_flag(ins.flags, ' ')
			|| check_flag(ins.flags, '+')));
}
