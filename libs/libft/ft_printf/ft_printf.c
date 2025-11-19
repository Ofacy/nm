/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcottet <lcottet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 16:41:42 by lcottet           #+#    #+#             */
/*   Updated: 2023/11/21 10:54:40 by lcottet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "ft_hex.h"
#include "ft_number.h"
#include "ft_char.h"

#include <stdarg.h>
#include <unistd.h>

int	ft_invalid_fmt(char *conversion, int *len, char *err)
{
	int	ret;

	*len = 0;
	ret = 0;
	if (conversion[0] == '\0' && *err)
		ret = ft_printchar('%');
	else if (conversion[0] != '\0')
		ret = ft_printchar('%');
	if (ret < 0)
		return (-1);
	*len += ret;
	*err = 1;
	if (conversion[0] == '\0')
		return (0);
	else
		ret = ft_printchar(conversion[0]);
	if (ret < 0)
		return (-1);
	*len += ret;
	return (1);
}

size_t	ft_printf_conversions(char *conversion, va_list args, int *len, char *e)
{
	if (conversion[0] == 'c')
		*len = ft_printchar(va_arg(args, int));
	else if (conversion[0] == 's')
		*len = ft_printstr(va_arg(args, char *));
	else if (conversion[0] == 'p')
		*len = ft_print_ptr(va_arg(args, unsigned long long), 0);
	else if (conversion[0] == 'i' || conversion[0] == 'd')
		*len = ft_putnbr_len_fd(va_arg(args, int), 1, 0);
	else if (conversion[0] == 'u')
		*len = ft_putunsigned_fd((unsigned int)va_arg(args, int), 1, 0);
	else if (conversion[0] == 'x')
		*len = ft_putnbr_base_fd(va_arg(args, int), "0123456789abcdef", 1, 0);
	else if (conversion[0] == 'X')
		*len = ft_putnbr_base_fd(va_arg(args, int), "0123456789ABCDEF", 1, 0);
	else if (conversion[0] == '\0' && !*e)
	{
		*len = -1;
		return (0);
	}
	else if (conversion[0] == '%')
		*len = ft_printchar('%');
	else
		return (ft_invalid_fmt(conversion, len, e));
	return (1);
}

int	ft_printf(const char *format, ...)
{
	va_list	list;
	int		print_len;
	int		ret;
	size_t	i;
	char	err;

	va_start(list, format);
	i = 0;
	print_len = 0;
	err = 0;
	while (format[i])
	{
		if (format[i] == '%')
		{
			i += 1 + ft_printf_conversions((char *)format + i + 1, list, &ret, \
				&err);
		}
		else
			ret = ft_printchar(format[i++]);
		if (ret < 0)
			return (-1);
		print_len += ret;
	}
	va_end(list);
	return (print_len);
}
