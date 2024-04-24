/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmelica <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 15:53:02 by hmelica           #+#    #+#             */
/*   Updated: 2023/11/03 15:39:13 by hmelica          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	handler(va_list act, t_insert ins, int fd[2]);
int	separation(const char **format, int ret[2],
		va_list act, va_list ori);

/*
main func
*/
int	ft_printf(const char *format, ...)
{
	int				ret[2];
	va_list			args;
	va_list			origin;
	va_list			act;

	va_start(act, format);
	va_copy(origin, act);
	ret[0] = 1;
	ret[1] = 0;
	while (format && *format)
	{
		if (*format == '%')
		{
			va_copy(args, origin);
			if (separation(&format, ret, act, args) < 0)
				return (va_end(act), va_end(origin), -1);
			va_end(args);
		}
		else if (++ret[1] && write(1, format, 1) < 0)
			return (va_end(act), va_end(origin), -1);
		format++;
	}
	return (va_end(act), va_end(origin), ret[1]);
}

/*
FLAGS : every binary 1 tells that the flag exists
1   '-' : left adjusted
2   '0' : zero padded
4   '#' : alternate form (???)
8   ' ' : blank before if positive
16  '+' : + before if positive

this func is handeling an insert, *format pointing at '%'
at the end, format is pointing at the csp
*/
int	separation(const char **format, int ret[2],
	va_list act, va_list ori)
{
	t_insert	ins;
	const char	*origin;

	origin = *format;
	ins.flags = 0;
	ins.min_width = 0;
	ins.prec = -1;
	while (++(*format) && is_flag(**format))
		ins.flags += (set_flag(**format) * !check_flag(ins.flags, **format));
	ins.min_width = get_min_width(format, &(ins.flags), act, ori);
	if (**format == '.')
		ins.prec = precision(format, act, ori);
	compile_flags(&ins);
	if (!is_csp(**format))
	{
		ret[1]++;
		*format = origin;
		if (write(ret[0], "%", 1) < 0)
			return (-1);
		return (1);
	}
	ins.type = **format;
	if (handler(act, ins, ret) < 0)
		return (-1);
	return (ret[1]);
}

/*
Launch different functions depending on the csp
Return number of char printed
*/
int	handler(va_list act, t_insert ins, int fd[2])
{
	int	ret;

	ret = 0;
	if (ins.type == 'c')
		ret = main_c(act, ins, fd[0]);
	else if (ins.type == 's')
		ret = main_s(act, ins, fd[0]);
	else if (ins.type == 'p')
		ret = main_p(act, ins, fd[0]);
	else if (ins.type == 'd' || ins.type == 'i')
		ret = main_d(act, ins, fd[0]);
	else if (ins.type == 'u')
		ret = main_u(act, ins, fd[0]);
	else if (ins.type == 'x')
		ret = main_x(act, ins, "0123456789abcdefx", fd[0]);
	else if (ins.type == 'X')
		ret = main_x(act, ins, "0123456789ABCDEFX", fd[0]);
	else if (ins.type == '%')
	{
		if (write(fd[0], "%", 1) < 0)
			return (-1);
		return (fd[1] += 1, 1);
	}
	return (fd[1] += ret, ret);
}
