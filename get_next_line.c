#include "get_next_line.h"
#include <stdio.h>
#include <string.h>

void	ft_strzdup2(t_fdarray *a, char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (a->o[a->y] != '\n')
		{
			a->o[a->y] = str[i];
			if (a->o[a->y] != '\n')
				a->y += 1;
		}
		a->swp[a->z] = str[i];
		if (!a->f && (a->swp[a->z] == '\n'))
		{
			a->z = 0;
			a->f = 1;
		}
		else
			a->z += 1;
		i += 1;
	}
}

char	*ft_strzdup(t_fdarray *a, int fd)
{
	if (!a->strs[fd])//&& !a->buffer[0])
	{
		a->strs[fd] = malloc(1);
		a->strs[fd][0] = 0;
	}
	a->i = 0;
	a->y = 0;
	while (a->strs[fd][a->i++])
		a->y += 1;
	a->i = 0;
	while (a->buffer[a->i++])
		a->y += 1;
	a->swp = malloc(a->y + 1);
	if (!a->swp)
		return (0);
	a->o = malloc(a->y + 1);
	if (!a->o)
		return (0);
	a->y = 0;
	a->z = 0;
	a->f = 0;
	ft_strzdup2(a, a->strs[fd]);
	ft_strzdup2(a, a->buffer);
	a->swp[a->z] = 0;
	a->buffer[0] = 0;
	free(a->strs[fd]);
	a->strs[fd] = a->swp;
	if (a->o[a->y] != '\n')
	{
	//	free(a->o);
		return (0);
	}
	a->o[a->y + 1] = 0;
	return (a->o);
}

char	*get_next_line(int fd)
{
	static t_fdarray	fdarray;
	char				*line;
	ssize_t				bytes;

	if (fd < 0)
		return (0);
	while (1)
	{
		line = ft_strzdup(&fdarray, fd);
		if (line)
			return (line);
		bytes = read(fd, fdarray.buffer, BUFFER_SIZE);
		if (bytes < 1)
			break ;
		fdarray.buffer[bytes] = 0;
	}
	if (!fdarray.is_end && fdarray.strs[fd][0])
	{
		fdarray.is_end = 1;
		return (fdarray.strs[fd]);
	}
	//if (!fdarray.is_end)
	//	free(fdarray.strs[fd]);
	fdarray.buffer[0] = 0;
	return (0);
}
