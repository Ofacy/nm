/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcottet <lcottet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 08:51:50 by lcottet           #+#    #+#             */
/*   Updated: 2023/11/07 09:05:02 by lcottet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	srclength;

	i = 0;
	srclength = 0;
	while (src[srclength])
	{
		if (i < size - 1 && size > 0)
		{
			dst[i] = src[i];
			i++;
		}
		srclength++;
	}
	if (size != 0)
	{
		dst[i] = '\0';
	}
	return (srclength);
}
