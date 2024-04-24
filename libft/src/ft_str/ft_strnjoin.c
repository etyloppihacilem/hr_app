/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 16:20:53 by nserve            #+#    #+#             */
/*   Updated: 2022/09/01 17:29:14 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

char			*ft_strnjoin(int size, char **strs, char *sep);
unsigned int	ft_strs_len(int size, char **strs, char *sep);
char			*ft_strcat(char *dest, char *src);

char	*ft_strnjoin(int size, char **strs, char *sep)
{
	char	*ret_str;
	int		total_len;
	int		i;

	if (!size || !strs || !sep)
		return (NULL);
	total_len = ft_strs_len(size, strs, sep);
	ret_str = malloc(sizeof(char) * (total_len + 1));
	if (!ret_str)
		return (NULL);
	ft_memset(ret_str, 0, total_len + 1);
	i = -1;
	while (++i < size)
	{
		ft_strcat(ret_str, strs[i]);
		if (i < size -1)
			ft_strcat(ret_str, sep);
	}
	return (ret_str);
}

unsigned int	ft_strs_len(int size, char **strs, char *sep)
{
	int				i;
	unsigned int	sep_len;
	unsigned int	total_len;

	sep_len = ft_strlen(sep);
	total_len = 0;
	i = -1;
	while (++i < size)
		total_len += ft_strlen(strs[i]) + sep_len;
	total_len -= sep_len;
	return (total_len);
}

char	*ft_strcat(char *dest, char *src)
{
	char	*origin;

	origin = dest;
	while (*dest)
	{
		dest++;
	}
	while (*src)
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return (origin);
}
