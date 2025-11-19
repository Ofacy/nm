/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcottet <lcottet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 23:48:01 by lcottet           #+#    #+#             */
/*   Updated: 2023/12/15 08:50:53 by lcottet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

void	*free_buff(char **buffer)
{
	free(*buffer);
	*buffer = NULL;
	return (NULL);
}

char	*ft_customstrjoin(char const *s1, char const *s2)
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
