/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 14:22:32 by nserve            #+#    #+#             */
/*   Updated: 2023/07/20 12:59:15 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi_base(char *str,	char *base);
int	ft_is_notbase(char *base);
int	ft_pos_char_base(char c, char *base);
int	ft_str_base_to_int(char *nbr, int nbr_len, char *base, int base_len);

int	ft_atoi_base(char *str, char *base)
{
	int		sign;
	int		base_len;
	int		nbr_len;

	if (!*str || ft_is_notbase(base))
		return (0);
	base_len = ft_strlen(base);
	while (*str == ' ' || ('\t' <= *str && *str <= '\r'))
		str++;
	sign = 1;
	while ((*str == '+' || *str == '-'))
	{
		if (*str == '-')
			sign = sign * -1;
		str++;
	}
	if (!ft_pos_char_base(*str, base))
		return (0);
	else
	{
		nbr_len = 0;
		while (ft_pos_char_base(str[nbr_len], base))
			nbr_len++;
		return (sign * ft_str_base_to_int(str, nbr_len, base, base_len));
	}
}

int	ft_is_notbase(char *base)
{
	int	i;

	if (ft_strlen(base) < 2)
		return (1);
	while (*base)
	{
		if (*base == '+' || *base == '-' || *base == ' ')
			return (1);
		i = 1;
		while (*base != base[i] && base[i])
			i++;
		if (*base == base[i])
			return (1);
		base++;
	}
	return (0);
}

int	ft_pos_char_base(char c, char *base)
{
	int	pos;

	pos = 1;
	while (*base)
	{
		if (c == *base)
			return (pos);
		base++;
		pos++;
	}
	return (0);
}

int	ft_str_base_to_int(char *nbr, int nbr_len, char *base, int base_len)
{
	int	dec_value;
	int	pwr_base;
	int	i;

	dec_value = 0;
	pwr_base = 1;
	i = 0;
	while (i++ < nbr_len)
	{
		dec_value += (ft_pos_char_base(nbr[nbr_len - i], base) - 1) * pwr_base;
		pwr_base *= base_len;
	}
	return (dec_value);
}
