/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:30:05 by emagnani          #+#    #+#             */
/*   Updated: 2024/05/28 16:46:44 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newlst;
	t_list	*node;
	t_list	*node_content;

	newlst = NULL;
	while (lst)
	{
		node_content = f(lst->content);
		node = ft_lstnew(node_content);
		if (!node)
		{
			del(node_content);
			break ;
		}
		lst = lst->next;
		ft_lstadd_back(&newlst, node);
	}
	return (newlst);
}
