/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 14:52:54 by jfreitas          #+#    #+#             */
/*   Updated: 2019/11/30 17:30:09 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*fresh_str;

	if (s1 && s2)
	{
		fresh_str = ft_strnew(ft_strlen(s1) + ft_strlen(s2));
		if (fresh_str == NULL || s1 == NULL || s2 == NULL)
			return (NULL);
		ft_strcpy(fresh_str, s1);
		ft_strcat(fresh_str, s2);
		return (fresh_str);
	}
	return (0);
}
