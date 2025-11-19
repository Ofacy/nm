/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcottet <lcottet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 23:47:46 by lcottet           #+#    #+#             */
/*   Updated: 2023/12/18 23:25:57 by lcottet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1042
#endif

char	*get_shortened_buffer(char *buffer, int torem)
{
	char	*new;
	size_t	newlen;

	newlen = ft_strlen(buffer + torem);
	if (newlen == 0)
	{
		free(buffer);
		return (NULL);
	}
	new = malloc(newlen + 1);
	if (!new)
	{
		free(buffer);
		return (NULL);
	}
	ft_strlcpy(new, buffer + torem, newlen + 1);
	free(buffer);
	return (new);
}

static char	*get_line(char **buffer)
{
	char	*line;
	char	*nlpos;
	size_t	line_length;

	nlpos = ft_strchr(*buffer, '\n');
	if (nlpos == 0)
		line_length = ft_strlen(*buffer);
	else
		line_length = (nlpos - (*buffer)) + 1;
	line = malloc(line_length + 1);
	if (!line)
	{
		free(*buffer);
		*buffer = NULL;
		return (NULL);
	}
	ft_strlcpy(line, *buffer, line_length + 1);
	if (nlpos == 0)
	{
		free(*buffer);
		*buffer = NULL;
		return (line);
	}
	*buffer = get_shortened_buffer(*buffer, line_length);
	return (line);
}

char	*append_buffer(char *buffer, char *new)
{
	char	*join;
	size_t	newlen;

	if (buffer == NULL)
	{
		newlen = ft_strlen(new);
		join = malloc(newlen + 1);
		if (!join)
		{
			free(new);
			return (NULL);
		}
		ft_strlcpy(join, new, newlen + 1);
		return (join);
	}
	join = ft_customstrjoin(buffer, new);
	if (!join)
	{
		free(buffer);
		free(new);
		return (NULL);
	}
	free(buffer);
	return (join);
}

char	*read_fd(char **buffer, int fd)
{
	char	*tmpbuffer;
	int		read_len;

	tmpbuffer = malloc(BUFFER_SIZE + 1);
	if (!tmpbuffer)
		return (free_buff(buffer));
	while (*buffer == NULL || !ft_strchr(*buffer, '\n'))
	{
		read_len = read(fd, tmpbuffer, BUFFER_SIZE);
		if (read_len == -1)
		{
			free(tmpbuffer);
			return (free_buff(buffer));
		}
		if (read_len == 0)
			return (free(tmpbuffer), NULL);
		tmpbuffer[read_len] = '\0';
		*buffer = append_buffer(*buffer, tmpbuffer);
		if (!*buffer)
			return (NULL);
	}
	return (free(tmpbuffer), *buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffers[__FD_SETSIZE] = {NULL};

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (read(fd, 0, 0) < 0)
	{
		if (buffers[fd])
			free(buffers[fd]);
		buffers[fd] = NULL;
		return (NULL);
	}
	if (buffers[fd] && ft_strchr(buffers[fd], '\n'))
		return (get_line(&buffers[fd]));
	read_fd(&buffers[fd], fd);
	if (!buffers[fd])
		return (NULL);
	return (get_line(&buffers[fd]));
}
