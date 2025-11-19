/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcottet <lcottet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 15:35:50 by lcottet           #+#    #+#             */
/*   Updated: 2023/11/09 21:28:06 by lcottet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>

static size_t	get_parts_count(char const *str, char c)
{
	size_t	parts;
	size_t	i;

	parts = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] == c && str[i])
			i++;
		if (str[i] != c && str[i])
			parts++;
		while (str[i] != c && str[i])
			i++;
		if (str[i] != 0)
			i++;
	}
	return (parts);
}

static size_t	get_word_length(char const *str, char c)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		while (str[i] != c && str[i])
			i++;
		if (str[i] == c || str[i] == 0)
			return (i);
		if (str[i] != 0)
			i++;
	}
	return (0);
}

static char	*get_word(char const *str, char c, size_t *reti)
{
	size_t	length;
	char	*ret;
	size_t	i;

	length = get_word_length(str, c);
	ret = malloc(length + 1);
	if (!ret)
		return (0);
	i = 0;
	while (i < length)
	{
		ret[i] = str[i];
		i++;
	}
	ret[i] = '\0';
	(*reti) += i;
	return (ret);
}

static char	**free_split_array(char **arr, size_t n)
{
	size_t	i;

	i = 0;
	while (arr[i] && i < n)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**output;
	size_t	i;
	size_t	splitsi;

	splitsi = get_parts_count(s, c);
	output = malloc(sizeof(char *) * (splitsi + 1));
	if (!output)
		return (0);
	output[splitsi] = 0;
	i = 0;
	splitsi = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (s[i] != c && s[i])
		{
			output[splitsi] = get_word(s + i, c, &i);
			if (output[splitsi++] == 0)
				return (free_split_array(output, splitsi - 1));
		}
		if (s[i])
			i++;
	}
	return (output);
}
