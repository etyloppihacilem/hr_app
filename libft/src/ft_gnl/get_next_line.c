/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 13:54:41 by nserve            #+#    #+#             */
/*   Updated: 2023/10/01 14:08:12 by hmelica          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*get_next_line(int fd)
{
	static t_lstash	*stash[4096];
	int				readsize;
	int				stashexist;

	if (fd < 0 || fd > 4096)
		return (NULL);
	stashexist = 0;
	readsize = fill_lstash(fd, &stash[fd]);
	if (stash[fd])
		stashexist = stash[fd]->buffersize;
	if (readsize == -1 || (!stashexist && !readsize))
	{
		return (clear_lstash(&stash[fd]), NULL);
	}
	if (readsize >= 0 && stashexist)
	{
		return (build_nl(&stash[fd]));
	}
	return (clear_lstash(&stash[fd]), NULL);
}

int	fill_lstash(int fd, t_lstash **p_lstash)
{
	t_lstash	*stash;
	int			readsize;

	if (*p_lstash && ((*p_lstash)->next || nl_len((*p_lstash)->buffer)))
		return (0);
	readsize = 0;
	while (readsize != -1)
	{
		stash = malloc(sizeof(*stash));
		if (!stash)
			return (-1);
		stash->next = NULL;
		readsize = read(fd, stash->buffer, BUFFER_SIZE);
		if (!readsize || readsize == -1)
		{
			clear_lstash(&stash);
			break ;
		}
		stash->buffer[readsize] = '\0';
		add_lstash(p_lstash, stash, readsize);
		if (nl_len(stash->buffer) || readsize < BUFFER_SIZE)
			break ;
	}
	return (readsize);
}

char	*build_nl(t_lstash **p_lstash)
{
	t_lstash	*current;
	char		*newline;
	size_t		crlen;
	size_t		linesize;

	linesize = 0;
	current = *p_lstash;
	while (current->next)
		current = current->next;
	crlen = nl_len(current->buffer);
	if (crlen)
		linesize = current->cumulsize - current->buffersize + crlen;
	else
		linesize = current->cumulsize;
	newline = malloc(sizeof(*newline) * (linesize + 1));
	if (!newline)
		return (NULL);
	copy_lstash(newline, p_lstash, crlen);
	*p_lstash = clean_lstash(p_lstash, crlen);
	return (newline);
}

void	copy_lstash(char *newline, t_lstash **p_lstash, size_t crlen)
{
	t_lstash	*current;
	int			i;
	int			j;

	current = *p_lstash;
	i = 0;
	while (current->next)
	{
		j = 0;
		while (current->buffer[j])
			newline[i++] = current->buffer[j++];
		current = current->next;
	}
	j = 0;
	if (crlen)
	{
		while ((size_t)j < crlen)
			newline[i++] = current->buffer[j++];
	}
	else
	{
		while (current->buffer[j])
			newline[i++] = current->buffer[j++];
	}
	newline[i] = '\0';
}

t_lstash	*clean_lstash(t_lstash **p_lstash, size_t crlen)
{
	t_lstash	*current;
	t_lstash	*newstash;
	int			i;

	newstash = malloc(sizeof(*newstash));
	if (!newstash)
		return (NULL);
	current = *p_lstash;
	while (current->next)
		current = current->next;
	i = 0;
	while (crlen && current->buffer[crlen + i])
	{
		newstash->buffer[i] = current->buffer[crlen + i];
		i++;
	}
	if (i == 0)
		return (free(newstash), clear_lstash(p_lstash), NULL);
	newstash->buffer[i] = '\0';
	newstash->buffersize = i;
	newstash->cumulsize = i;
	newstash->next = NULL;
	newstash->buffer[crlen + i] = '\0';
	return (clear_lstash(p_lstash), newstash);
}
