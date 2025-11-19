/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hex.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcottet <lcottet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 13:55:04 by lcottet           #+#    #+#             */
/*   Updated: 2023/11/20 13:00:20 by lcottet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_char.h"
#include <unistd.h>

int	ft_putnbr_base_fd(unsigned int nbr, char *base, int fd, int inlen)
{
	size_t	base_length;
	int		writesize;

	base_length = (size_t)ft_strlen(base);
	if (base_length < 2)
		return (-1);
	if (nbr >= base_length)
	{
		inlen = ft_putnbr_base_fd(nbr / base_length, base, fd, inlen);
		if (inlen < 0)
			return (-1);
	}
	writesize = ft_printchar(base[nbr % base_length]);
	if (writesize < 0)
		return (-1);
	return (writesize + inlen);
}

int	ft_print_ptr(unsigned long long nbr, int inlen)
{
	int		writesize;
	char	*base;

	base = "0123456789abcdef";
	if (nbr == 0)
		return (ft_printstr("(nil)"));
	if (inlen == 0)
	{
		writesize = ft_printstr("0x");
		if (writesize < 0)
			return (-1);
		inlen += writesize;
	}
	if (nbr >= 16)
	{
		inlen = ft_print_ptr(nbr / 16, inlen);
		if (inlen < 0)
			return (-1);
	}
	writesize = ft_printchar(base[nbr % 16]);
	if (writesize < 0)
		return (-1);
	return (writesize + inlen);
}
