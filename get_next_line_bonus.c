/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafaussu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 12:31:15 by mafaussu          #+#    #+#             */
/*   Updated: 2022/02/15 12:34:05 by mafaussu         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 10
#endif

typedef struct s_pstr
{
	ssize_t	l;
	char	*s;
}	t_pstr;

typedef struct s_fda
{
	t_pstr	prev[OPEN_MAX];
	char	buffer[BUFFER_SIZE];
	ssize_t	l;
}	t_fda;

t_pstr	pstr_join(t_pstr a, t_pstr b)
{
	t_pstr	o;
	ssize_t	i;
	ssize_t	y;

	o.l = a.l + b.l;
	if (o.l)
		o.s = malloc(o.l + 1);
	else
		o.s = 0;
	i = 0;
	while (i < a.l)
	{
		o.s[i] = a.s[i];
		i += 1;
	}
	y = 0;
	while (y < b.l)
	{
		o.s[i] = b.s[y];
		y += 1;
		i += 1;
	}
	if (o.l)
		o.s[i] = 0;
	return (o);
}

t_pstr	pstr_sub(t_pstr s, ssize_t f, ssize_t l)
{
	t_pstr	o;
	ssize_t	y;
	ssize_t	i;

	o.l = l;
	if (o.l)
		o.s = malloc(o.l + 1);
	else
		o.s = 0;
	i = 0;
	y = 0;
	while (i < s.l && y < l)
	{
		if (i >= f && y < l)
			o.s[y++] = s.s[i];
		i += 1;
	}
	if (o.l)
		o.s[y] = 0;
	return (o);
}

ssize_t	pstr_chr(t_pstr s, char c)
{
	ssize_t	i;

	i = 0;
	while (i < s.l)
	{
		if (s.s[i] == c)
			return (i);
		i += 1;
	}
	return (-1);
}

char	*parse_read(t_fda *fda, int fd, char **o)
{
	t_pstr	swp;

	*o = (char *) 42;
	if (fda->l == 0)
	{
		if (fda->prev[fd].l)
		{
			swp = fda->prev[fd];
			fda->prev[fd].l = 0;
			fda->prev[fd].s = 0;
			*o = swp.s;
			return (*o);
		}
		*o = 0;
	}
	else if (fda->l < 0)
	{
		if (fda->prev[fd].l)
		{
			free(fda->prev[fd].s);
			fda->prev[fd] = (t_pstr){.s = 0, .l = 0};
		}
		*o = 0;
	}
	return (*o);
}

char	*get_next_line(int fd)
{
	static t_fda	fda;
	t_pstr			swp;
	t_pstr			out;
	ssize_t			i;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	while (1)
	{
		i = pstr_chr(fda.prev[fd], '\n');
		if (i > -1)
		{
			out = pstr_sub(fda.prev[fd], 0, i + 1);
			swp = pstr_sub(fda.prev[fd], i + 1, fda.prev[fd].l - i - 1);
			free(fda.prev[fd].s);
			fda.prev[fd] = swp;
			return (out.s);
		}
		fda.l = read(fd, fda.buffer, BUFFER_SIZE);
		if (parse_read(&fda, fd, &swp.s) != (char *) 42)
			return (swp.s);
		swp = pstr_join(fda.prev[fd], (t_pstr){.l = fda.l, .s = fda.buffer});
		free(fda.prev[fd].s);
		fda.prev[fd] = swp;
	}
}
