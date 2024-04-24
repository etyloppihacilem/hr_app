/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 16:44:22 by nserve            #+#    #+#             */
/*   Updated: 2023/10/01 14:08:17 by hmelica          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	nl_len(const char *str)
{
	size_t	size;

	if (!str)
		return (0);
	size = 0;
	while (str[size])
	{
		if (str[size] == '\n')
			return (size + 1);
		size++;
	}
	return (0);
}

void	clear_lstash(t_lstash **p_lstash)
{
	t_lstash	*current;
	t_lstash	*todel;

	if (!p_lstash)
		return ;
	current = *p_lstash;
	while (current)
	{
		todel = current;
		current = current->next;
		free(todel);
	}
	p_lstash = NULL;
	return ;
}

void	add_lstash(t_lstash **p_lstash, t_lstash *newstash, size_t readsize)
{
	t_lstash	*current;

	if (!*p_lstash)
	{
		*p_lstash = newstash;
		newstash->buffersize = readsize;
		newstash->cumulsize = readsize;
		return ;
	}
	current = *p_lstash;
	while (current && current->next)
		current = current->next;
	current->next = newstash;
	newstash->buffersize = readsize;
	newstash->cumulsize = current->cumulsize + readsize;
	return ;
}
