/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 15:49:40 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/15 19:35:40 by whoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	*ft_memccpy(void *dst, void const *src, int c, size_t n)
{
	size_t			i;
	unsigned char	*dest;
	unsigned char	*str;

	i = 0;
	dest = (unsigned char *)dst;
	str = (unsigned char *)src;
	while (i < n)
	{
		dest[i] = str[i];
		if (str[i] == (unsigned char)c)
			return (dst + i + 1);
		i++;
	}
	return (NULL);
}
