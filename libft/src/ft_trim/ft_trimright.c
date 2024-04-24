/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trimright.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 19:08:50 by nserve            #+#    #+#             */
/*   Updated: 2023/01/20 19:09:29 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_trimright(const char *str)
{
	char			*ret;
	unsigned int	start;
	size_t			len;

	start = 0;
	len = ft_strlen(str) -1;
	while (ft_isspace(str[len]))
		len--;
	ret = ft_substr(str, start, len);
	return (ret);
}
