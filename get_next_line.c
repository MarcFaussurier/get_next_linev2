#include "get_next_line.h"
#include <stdio.h>
#include <string.h>

char	*ft_strzdup(char **s1, char *s2)
{
	size_t	z;
	size_t	y;
	size_t	i;
	int		f;
	int		r0;
	char	*o;
	char	*swp;

	r0 = 0;
	if (!*s1 && !s2[0])
	{
		*s1 = malloc(1);
		**s1 = 0;
		r0 = 1;
	}
	i = 0;
	y = 0;
	while ((*s1)[i++])
		y += 1;
	i = 0;
	while (s2[i++])
		y += 1;
	swp = malloc(y + 1);
	if (!swp)
		return (0);
	o = malloc(y + 1);
	if (!o)
		return (0);
	i = 0;
	y = 0;
	z = 0;
	f = 0;
	while ((*s1)[i])
	{
		if (o[y] != '\n')
		{
			o[y] = (*s1)[i];
			if (o[y] != '\n')
				y += 1;
		}
		swp[z] = (*s1)[i];
		if (!f && (swp[z] == '\n'))
		{
			z = 0;
			f = 1;
		}
		else
			z += 1;
		i += 1;
	}
	i = 0;
	while ((s2)[i])
	{
		if (o[y] != '\n')
		{
			o[y] = (s2)[i];
			if (o[y] != '\n')
				y += 1;
		}
		swp[z] = s2[i];
		if (!f && (swp[z] == '\n'))
		{
			z = 0;
			f = 1;
		}
		else
			z += 1;
		i += 1;
	}
	swp[z] = 0;
	s2[0] = 0;
	free(*s1);
	*s1 = swp;
	if (o[y] != '\n')
	{
		free(o);
		return (0);
	}
	o[y + 1] = 0;
	return (o);
}

char	*get_next_line(int fd)
{
	static t_fdarray	fdarray;
	char				*line;
	ssize_t				bytes;

	while (1)
	{
		line = ft_strzdup(&fdarray.strs[fd], fdarray.buffer);
		if (line)
			return (line);
		bytes = read(fd, fdarray.buffer, BUFFER_SIZE);
		if (bytes < 1)
			break ;
		fdarray.buffer[bytes] = 0;
	}
	if (fdarray.strs[fd][0])
	{
		line = (strdup(fdarray.strs[fd]));
		fdarray.strs[fd][0] = 0;
		return (line);
	}
	free(fdarray.strs[fd]);
	fdarray.buffer[0] = 0;
	return (0);
}
