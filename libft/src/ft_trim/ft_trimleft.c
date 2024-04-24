/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trimleft.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 18:55:44 by nserve            #+#    #+#             */
/*   Updated: 2023/01/20 19:27:54 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_trimleft(const char *str)
{
	char			*ret;
	unsigned int	start;
	size_t			len;

	start = 0;
	while (ft_isspace(str[start]))
		start++;
	len = ft_strlen(str + start);
	ret = ft_substr(str, start, len);
	return (ret);
}
