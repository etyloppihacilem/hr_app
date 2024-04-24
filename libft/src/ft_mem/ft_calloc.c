/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 12:25:21 by nserve            #+#    #+#             */
/*   Updated: 2022/11/23 12:12:16 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_product_ovf(size_t a, size_t b);

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ret;

	if (nmemb == 0 || size == 0)
		return (malloc(0));
	else if (ft_product_ovf(nmemb, size))
		return (NULL);
	else
	{
		ret = malloc(nmemb * size);
		if (!ret)
			return (NULL);
		ft_bzero(ret, nmemb * size);
		return (ret);
	}
}

static int	ft_product_ovf(size_t a, size_t b)
{
	if ((b != 0 && a > SIZE_MAX / b) || (a != 0 && b > SIZE_MAX / a))
		return (1);
	else
		return (0);
}		
