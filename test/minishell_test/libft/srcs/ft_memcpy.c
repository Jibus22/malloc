/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 11:28:47 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/15 20:22:29 by whoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	*ft_memcpy(void *dst, void const *src, size_t n)
{
	size_t		i;
	char		*dest;
	char const	*str;

	i = 0;
	dest = (char *)dst;
	str = (char const *)src;
	if (n == 0 || dst == src)
		return (dst);
	while (dst != NULL || src != NULL)
	{
		while (i < n)
		{
			dest[i] = str[i];
			i++;
		}
		return (dst);
	}
	return (NULL);
}
