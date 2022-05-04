/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_jb_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 20:46:28 by whoami            #+#    #+#             */
/*   Updated: 2021/03/15 20:49:43 by whoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** #1. L’adresse du pointeur vers un élément.
** #2. L’adresse de la fonction permettant de supprimer le contenu d’un élément.
**
** Supprime et libère la mémoire de l’élément passé en paramètre, et de tous les
** élements qui suivent, à l’aide de del et de free(3). Enfin, le pointeur
** initial doit être mis à NULL.
*/
void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;

	if (!(*lst) || !del)
		return ;
	while ((*lst))
	{
		tmp = *lst;
		(*del)((*lst)->content);
		*lst = (*lst)->next;
		free(tmp);
		tmp = NULL;
	}
}
