/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_jb_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 20:47:39 by whoami            #+#    #+#             */
/*   Updated: 2021/03/15 20:49:01 by whoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*newlink;

	newlink = (t_list *)malloc(sizeof(t_list));
	if (!newlink)
		return (NULL);
	newlink->content = content;
	newlink->next = NULL;
	return (newlink);
}
