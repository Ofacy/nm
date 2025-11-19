/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcottet <lcottet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 13:03:11 by lcottet           #+#    #+#             */
/*   Updated: 2023/11/09 21:27:47 by lcottet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	str_len;
	size_t	output_size;
	char	*output;

	str_len = ft_strlen(s);
	if (start >= str_len)
		return (ft_strdup(""));
	output_size = len + 1;
	if (str_len - start < len)
		output_size = str_len - start + 1;
	output = malloc(output_size);
	if (!output)
		return (0);
	ft_strlcpy(output, s + start, output_size);
	return (output);
}
