/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strclen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 14:50:19 by jfreitas          #+#    #+#             */
/*   Updated: 2021/02/19 14:50:27 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

size_t	ft_strclen(char const *s, int c)
{
	size_t	len;

	len = 0;
	while (s[len] != c && s[len])
		len++;
	return (len);
}
