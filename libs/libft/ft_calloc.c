/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcottet <lcottet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 12:35:24 by lcottet           #+#    #+#             */
/*   Updated: 2023/11/09 21:27:11 by lcottet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdint.h>
#include <stdlib.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	totalsize;
	void	*mem;

	if (size != 0 && nmemb > SIZE_MAX / size)
		return (0);
	totalsize = nmemb * size;
	mem = malloc(totalsize);
	if (!mem)
		return (NULL);
	ft_bzero(mem, totalsize);
	return (mem);
}
