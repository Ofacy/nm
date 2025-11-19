/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcottet <lcottet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 09:25:24 by lcottet           #+#    #+#             */
/*   Updated: 2023/11/07 10:12:30 by lcottet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	foundi;
	size_t	i;
	char	foundone;

	foundone = 0;
	i = 0;
	foundi = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
		{
			foundone = 1;
			foundi = i;
		}
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)s + i);
	if (foundone == 1)
	{
		return ((char *)s + foundi);
	}
	return (0);
}
