/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 11:14:11 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/15 19:36:39 by whoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

int	ft_strcmp(char const *s1, char const *s2)
{
	while ((*s1 && *s2) && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	if (*s1 == *s2)
		return (0);
	else
		return (*((unsigned char *)s1) - *((unsigned char *)s2));
}
