/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/10 17:14:10 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/15 19:45:44 by whoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	*ft_memmove(void *dst, void const *src, size_t len)
{
	size_t	i;

	i = 0;
	if (dst == src || len == 0)
		return (dst);
	while (dst != NULL || src != NULL)
	{
		if ((char const *)src < (char *)dst)
			while (len-- > 0)
				*((char *)dst + len) = *((char const *)src + len);
		else
		{
			while (i < len)
			{
				*((char *)dst + i) = *((char const *)src + i);
				i++;
			}
		}
		return (dst);
	}
	return (NULL);
}
