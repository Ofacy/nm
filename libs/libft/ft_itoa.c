/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcottet <lcottet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 16:55:01 by lcottet           #+#    #+#             */
/*   Updated: 2023/11/09 21:28:14 by lcottet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>

static void	write_nbr(char *dest, int nbr)
{
	if (nbr >= 10 || nbr <= -10)
	{
		if (nbr < 0)
			write_nbr(dest - 1, -(nbr / 10));
		else
			write_nbr(dest - 1, nbr / 10);
	}
	if (nbr >= 0)
		*dest = (nbr % 10) + '0';
	else
		*dest = -(nbr % 10) + '0';
}

char	*ft_itoa(int n)
{
	size_t	strlength;
	int		ncopy;
	char	*output;

	strlength = 1;
	ncopy = n;
	if (n < 0)
		strlength++;
	while (ncopy >= 10 || ncopy <= -10)
	{
		ncopy /= 10;
		strlength++;
	}
	output = malloc(strlength + 1);
	if (!output)
		return (0);
	if (n < 0)
		output[0] = '-';
	write_nbr(output + strlength - 1, n);
	output[strlength] = '\0';
	return (output);
}
