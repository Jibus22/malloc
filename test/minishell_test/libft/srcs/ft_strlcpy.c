/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/14 14:09:13 by jfreitas          #+#    #+#             */
/*   Updated: 2019/11/15 15:21:56 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

size_t	ft_strlcpy(char *dst, char const *src, size_t dstsize)
{
	size_t	i;
	size_t	srcl;

	if (dst && src)
	{
		i = 0;
		srcl = ft_strlen(src);
		if (dstsize == 0)
			return (srcl);
		while (src[i] != '\0' && i < dstsize - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
		return (srcl);
	}
	return (0);
}
