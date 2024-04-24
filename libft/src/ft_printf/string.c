/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmelica <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 18:39:28 by hmelica           #+#    #+#             */
/*   Updated: 2023/11/03 15:18:09 by hmelica          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
return 1 if free is needed on buffer
*/
static int	generate_buffer(char **buffer, char *s, t_insert ins)
{
	if (!s && (ins.prec < 0 || ins.prec >= 6))
		*buffer = "(null)";
	else if (!s)
		*buffer = "";
	else if (ins.prec >= 0)
	{
		*buffer = malloc(ins.prec + 1);
		if (!*buffer)
			return (-1);
		ft_strlcpy(*buffer, s, ins.prec + 1);
		return (1);
	}
	else
		*buffer = s;
	return (0);
}

/*
Writes a string
*/
int	main_s(va_list act, t_insert ins, int fd)
{
	char	*buffer;
	int		ret;
	int		need_free;

	need_free = generate_buffer(&buffer, va_arg(act, char *), ins);
	if (need_free < 0)
		return (0);
	ret = display(buffer, ins, 0, fd);
	if (need_free == 1)
		free(buffer);
	return (ret);
}
