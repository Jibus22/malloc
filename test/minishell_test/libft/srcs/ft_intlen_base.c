/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_intlen_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 17:03:05 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/15 19:26:19 by whoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

int	ft_intlen_base(unsigned long n, int base)
{
	int	len;

	len = 0;
	if (n < 0)
		n *= -1;
	if (n == 0)
		len = 1;
	while (n)
	{
		n = n / base;
		len++;
	}
	return (len);
}
