/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 15:11:35 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/15 19:32:48 by whoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strnstr(char const *s, char const *needle, size_t l)
{
	size_t	j;
	char	*str;
	char	*to_find;
	int		length;

	j = 0;
	str = (char *)s;
	to_find = (char *)needle;
	length = ft_strlen(to_find);
	if (!*to_find)
		return (str);
	while (str[j] && (j + length) <= l)
	{
		if (ft_strncmp(s + j, needle, length) == 0)
			return (str + j);
		j++;
	}
	return (NULL);
}
