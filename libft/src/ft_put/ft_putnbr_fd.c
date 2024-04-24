/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 12:28:02 by nserve            #+#    #+#             */
/*   Updated: 2022/11/17 17:00:03 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putchar_fd("0123456789"[-(n % 10)], fd);
	}
	else if (n < 0)
	{
		ft_putchar_fd('-', fd);
		ft_putnbr_fd(-1 * n, fd);
	}
	else if (n < 10)
		ft_putchar_fd("0123456789"[n], fd);
	else
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putchar_fd("0123456789"[n % 10], fd);
	}
}
