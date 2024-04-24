/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 11:55:11 by nserve            #+#    #+#             */
/*   Updated: 2022/11/24 11:05:23 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void		ft_putnbr_str(char *dst, int n, size_t size);
static size_t	ft_nblen(int n);

char	*ft_itoa(int n)
{
	char	*ret;
	size_t	nbsize;

	nbsize = ft_nblen(n);
	ret = malloc(sizeof(*ret) * (nbsize + 1));
	if (!ret)
		return (NULL);
	ret[nbsize] = '\0';
	ft_putnbr_str(ret, n, nbsize);
	return (ret);
}

static void	ft_putnbr_str(char *dst, int n, size_t pos)
{
	if (n == -2147483648)
	{
		ft_putnbr_str(dst, n / 10, pos - 1);
		dst[pos - 1] = "0123456789"[-(n % 10)];
	}
	else if (n < 0)
	{
		dst[0] = '-';
		ft_putnbr_str(dst, -1 * n, pos);
	}
	else if (n < 10)
		dst[pos - 1] = "0123456789"[n];
	else
	{
		ft_putnbr_str(dst, n / 10, pos - 1);
		dst[pos - 1] = "0123456789"[n % 10];
	}
	return ;
}

static size_t	ft_nblen(int n)
{
	size_t	size;

	size = 0;
	if (n == 0)
		return (1);
	if (n == -2147483648)
	{
		n /= 10;
		size++;
	}
	if (n < 0)
	{
		n *= -1;
		size++;
	}
	while (n > 0)
	{
		n /= 10;
		size++;
	}
	return (size);
}
