/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnequ.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 15:02:14 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/15 19:31:14 by whoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

int	ft_strnequ(char const *s1, char const *s2, size_t n)
{
	size_t	i;

	if (s1 && s2)
	{
		i = 0;
		if (ft_strncmp(&s1[i], &s2[i], n) == 0)
		{
			return (1);
			i++;
		}
	}
	return (0);
}
