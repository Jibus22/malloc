/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 16:36:04 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/15 19:44:35 by whoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*allocating_mem(size_t len)
{
	char	*fresh_str;

	fresh_str = (char *)malloc(sizeof(char) * len + 1);
	if (!fresh_str)
		return (NULL);
	fresh_str[len] = '\0';
	return (fresh_str);
}

char	*ft_itoa(int n)
{
	size_t	len;
	size_t	neg;
	char	*fresh_str;

	len = ft_intlen(n);
	fresh_str = allocating_mem(len);
	if (!fresh_str)
		return (NULL);
	if (n < 0)
	{
		fresh_str[0] = '-';
		neg = 1;
	}
	else
		neg = 0;
	while (len > neg)
	{
		len--;
		if (n < 0)
			fresh_str[len] = 48 + n % 10 * -1;
		else
			fresh_str[len] = 48 + n % 10 * 1;
		n = n / 10;
	}
	return (fresh_str);
}
