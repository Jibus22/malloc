/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_jb.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 18:04:52 by jle-corr          #+#    #+#             */
/*   Updated: 2021/03/15 21:28:04 by whoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	split_counter(char const *s, char c)
{
	char	*spliter;
	int		i;

	i = 0;
	while (s && *s)
	{
		spliter = ft_strchr(s, c);
		if ((spliter - s) != 0)
			i++;
		if (spliter)
			s = spliter + 1;
		else
			s = NULL;
	}
	return (i + 1);
}

static char const	*ternarie_ret(char const *s, char *adr)
{
	if (adr)
		s = adr + 1;
	else
		s = NULL;
	return (s);
}

char	**ft_split_jb(char const *s, char c)
{
	char	**tab;
	char	*adr;
	int		i;

	tab = (char **)malloc(sizeof(*tab) * split_counter(s, c));
	if (!tab)
		return (NULL);
	i = 0;
	while (s && *s)
	{
		adr = ft_strchr(s, c);
		if ((adr - s) != 0)
		{
			if (adr)
				tab[i] = ft_substr(s, 0, adr - s);
			else
				tab[i] = ft_substr(s, 0, ft_strlen(s));
			if (!tab[i++])
				return (ft_freetab(tab));
		}
		s = ternarie_ret(s, adr);
	}
	tab[i] = NULL;
	return (tab);
}
