/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_jb_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 20:45:18 by whoami            #+#    #+#             */
/*   Updated: 2021/03/15 20:52:16 by whoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list	*last;

	last = ft_lstlast(*alst);
	if (!new)
		return ;
	if (!last)
		ft_lstadd_front(alst, new);
	else
		last->next = new;
}
