/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmelica <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 18:39:28 by hmelica           #+#    #+#             */
/*   Updated: 2023/09/24 11:06:12 by hmelica          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Write down a char
*/
int	main_c(va_list act, t_insert ins, int fd)
{
	unsigned char	c;

	c = va_arg(act, int);
	return (display((char *)&c, ins, 1, fd));
}
