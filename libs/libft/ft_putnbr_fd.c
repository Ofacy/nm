/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcottet <lcottet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:45:58 by lcottet           #+#    #+#             */
/*   Updated: 2023/11/09 21:28:31 by lcottet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

void	ft_putnbr_fd(int n, int fd)
{
	if (n < 0)
	{
		write(fd, "-", 1);
	}
	if (n > 9)
		ft_putnbr_fd(n / 10, fd);
	else if (n < -9)
		ft_putnbr_fd(-(n / 10), fd);
	if (n > 0)
		ft_putchar_fd(n % 10 + '0', fd);
	else
		ft_putchar_fd(-(n % 10) + '0', fd);
}
