/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcottet <lcottet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 11:24:30 by lcottet           #+#    #+#             */
/*   Updated: 2023/11/09 21:05:58 by lcottet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

static long	get_num(const char *nptr, size_t i, char sign)
{
	long	output;

	output = 0;
	while (ft_isdigit(nptr[i]))
	{
		if (output * sign != (output * 10 + nptr[i] - '0') / (10 * sign))
		{
			if (sign == 1)
				return (LONG_MAX);
			return (LONG_MIN);
		}
		output = output * 10 + nptr[i] - '0';
		i++;
	}
	return (output);
}

int	ft_atoi(const char *nptr)
{
	size_t	i;
	long	output;
	char	sign;

	i = 0;
	sign = 1;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '-')
	{
		i++;
		sign = -1;
	}
	else if (nptr[i] == '+')
		i++;
	output = get_num(nptr, i, sign);
	return ((int)(output * sign));
}
