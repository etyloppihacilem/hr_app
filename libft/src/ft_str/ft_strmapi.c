/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 12:27:43 by nserve            #+#    #+#             */
/*   Updated: 2022/11/24 11:13:11 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(const char *s, char (*f)(unsigned int, char))
{
	char	*ret;
	size_t	size;
	int		i;

	size = ft_strlen(s);
	ret = malloc(sizeof(*ret) * (size + 1));
	if (!ret || !s)
		return (NULL);
	ret[size] = '\0';
	i = 0;
	while (s[i])
	{
		ret[i] = f(i, s[i]);
		i++;
	}
	return (ret);
}
