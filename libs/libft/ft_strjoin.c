/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcottet <lcottet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 13:31:57 by lcottet           #+#    #+#             */
/*   Updated: 2023/11/09 21:27:56 by lcottet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	strlen;
	char	*str;

	strlen = ft_strlen(s1) + ft_strlen(s2) + 1;
	str = malloc(strlen);
	if (!str)
		return (NULL);
	ft_strlcpy(str, s1, strlen);
	ft_strlcat(str, s2, strlen);
	return (str);
}
