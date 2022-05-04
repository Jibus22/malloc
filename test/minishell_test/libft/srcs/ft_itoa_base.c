/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 16:36:04 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/15 20:34:27 by whoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

static char	ft_calculate_char(int res, char c)
{
	char	ret_char;

	ret_char = '0';
	while (res)
	{
		ret_char++;
		if (ret_char == ':')
			ret_char = c;
		res--;
	}
	return (ret_char);
}

char	*nb_is_zero(unsigned long n, int n_len, int base)
{
	char	*str;

	if (n == 0)
	{
		str = ft_strnew(1);
		if (!str)
			return (NULL);
		*str = '0';
		return (str);
	}
	n_len = ft_intlen_base(n, base);
	str = ft_strnew(n_len);
	return (str);
}

char	*ft_itoa_base(unsigned long n, int base, char c)
{
	unsigned long	nb;
	int				res;
	int				n_len;
	char			*str;

	str = nb_is_zero(n, n_len, base);
	if (str == NULL)
		return (NULL);
	nb = n;
	if (!str)
		return (NULL);
	while (nb)
	{
		res = nb % base;
		nb /= base;
		str[n_len - 1] = ft_calculate_char(res, c);
		n_len--;
	}
	return (str);
}
