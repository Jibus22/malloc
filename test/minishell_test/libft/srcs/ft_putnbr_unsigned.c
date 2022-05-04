/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_unsigned.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/25 13:26:03 by jfreitas          #+#    #+#             */
/*   Updated: 2021/02/18 23:48:13 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	ft_putnbr_unsigned(unsigned int n)
{
	if (n > 4294967295 || n < 0)
		return ;
	if (n > 9)
	{
		ft_putnbr_unsigned(n / 10);
		ft_putnbr_unsigned(n % 10);
	}
	else
		ft_putchar(n + 48);
}
