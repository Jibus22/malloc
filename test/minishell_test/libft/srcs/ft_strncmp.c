/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/11 13:46:46 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/15 19:33:45 by whoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

int	ft_strncmp(char const *s1, char const *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i] && i < n)
		i++;
	if (i < n)
		return (*((unsigned char *)s1 + i) - *((unsigned char *)s2 + i));
	return (0);
}
