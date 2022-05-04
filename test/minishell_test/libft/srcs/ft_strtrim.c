/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 16:35:13 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/15 19:46:30 by whoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		len;
	char	*str;

	if (s1)
	{
		start = 0;
		while (s1[start] && ft_strchr(set, s1[start]) != NULL)
			start++;
		len = ft_strlen(&s1[start]);
		if (len != 0)
			while (s1[start + len - 1]
				&& ft_strchr(set, s1[start + len - 1]) != NULL)
				len--;
		str = ft_substr(s1, start, len);
		if (s1 == NULL || str == NULL || set == NULL)
			return (NULL);
		return (str);
	}
	return (0);
}
