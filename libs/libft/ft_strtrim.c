/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcottet <lcottet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 13:42:41 by lcottet           #+#    #+#             */
/*   Updated: 2023/11/07 16:35:45 by lcottet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	starttrim;
	size_t	endtrim;
	size_t	s1len;

	s1len = ft_strlen(s1);
	starttrim = 0;
	while (s1[starttrim] && ft_strchr(set, s1[starttrim]) != NULL)
		starttrim++;
	endtrim = 0;
	while (s1[s1len - endtrim - 1] \
		&& ft_strchr(set, s1[s1len - endtrim - 1]) != NULL)
		endtrim++;
	return (ft_substr(s1, starttrim, s1len - endtrim - starttrim));
}
