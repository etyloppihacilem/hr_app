/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 10:45:12 by nserve            #+#    #+#             */
/*   Updated: 2023/10/15 14:35:18 by hmelica          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin2(char *s1, char *s2, int free_s1, int free_s2)
{
	char	*ret;
	char	*o1;
	char	*o2;
	int		i;

	if (!s1 && !s2)
		return (NULL);
	o1 = s1;
	o2 = s2;
	ret = malloc(sizeof(*ret) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!ret)
		return (NULL);
	i = 0;
	while (s1 && *s1)
		ret[i++] = *(s1++);
	while (s2 && *s2)
		ret[i++] = *(s2++);
	ret[i] = '\0';
	if (o1 && free_s1)
		free(o1);
	if (o2 && free_s2)
		free(o2);
	return (ret);
}
