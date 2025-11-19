/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_number.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcottet <lcottet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 14:20:49 by lcottet           #+#    #+#             */
/*   Updated: 2023/11/20 13:01:37 by lcottet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_char.h"
#include <unistd.h>

int	ft_putnbr_len_fd(int n, int fd, int len)
{
	int	print_len;

	if (n < 0)
		len += ft_printchar('-');
	if (len < 0)
		return (-1);
	if (n > 9)
		len = ft_putnbr_len_fd(n / 10, fd, len);
	else if (n < -9)
		len = ft_putnbr_len_fd(-(n / 10), fd, len);
	if (len < 0)
		return (-1);
	if (n >= 0)
		print_len = ft_printchar(n % 10 + '0');
	else
		print_len = ft_printchar(-(n % 10) + '0');
	if (print_len < 0)
		return (print_len);
	len += print_len;
	return (len);
}

int	ft_putunsigned_fd(unsigned int n, int fd, int len)
{
	int	print_len;

	if (len < 0)
		return (-1);
	if (n > 9)
		len = ft_putunsigned_fd(n / 10, fd, len);
	if (len < 0)
		return (-1);
	print_len = ft_printchar(n % 10 + '0');
	if (print_len < 0)
		return (-1);
	return (len + print_len);
}
