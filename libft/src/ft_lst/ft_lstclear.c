/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nserve <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 17:59:08 by nserve            #+#    #+#             */
/*   Updated: 2022/11/24 13:17:53 by nserve           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*current;
	t_list	*todel;

	if (!lst || !del)
		return ;
	current = *lst;
	while (current)
	{
		todel = current;
		current = current->next;
		ft_lstdelone(todel, del);
	}
	*lst = NULL;
	return ;
}
