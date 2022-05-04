/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 14:51:44 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/15 19:33:19 by whoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strstr(char const *haystack, char const *needle)
{
	size_t	i;
	size_t	j;
	char	*str;
	char	*to_find;

	j = 0;
	i = 1;
	str = (char *)haystack;
	to_find = (char *)needle;
	if (!*to_find)
		return (str);
	while (str[j])
	{
		if (str[j] == to_find[0])
		{
			while (to_find[i] && str[j + i] == to_find[i])
				i++;
			if (!to_find[i])
				return (&str[j]);
		}
		j++;
	}
	return (NULL);
}
