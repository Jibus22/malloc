/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 13:28:13 by jfreitas          #+#    #+#             */
/*   Updated: 2019/11/21 15:21:36 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	*ft_memalloc(size_t size)
{
	void	*fresh_mem;

	fresh_mem = malloc(sizeof(void) * size);
	if (fresh_mem == NULL)
		return (NULL);
	else
		ft_bzero(fresh_mem, size);
	return (fresh_mem);
}
