#include <unistd.h>
#include <stdlib.h>
#include <limits.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 80
#endif

/*
 *		TODO: 	- [ ] Use linked list for t_string to optimize 
 *				string_join and string_sub
 */

typedef struct s_string
{
	char	*s;
	ssize_t	l;

}			t_string;
typedef struct s_fda
{
	t_string	prev[OPEN_MAX];
	char		buffer[BUFFER_SIZE + 1];
}		t_fda;

static t_string	string_join(t_string s, t_string b)
{
	ssize_t		i;
	ssize_t		y;
	t_string	o;

	o.l = s.l + b.l;
	o.s = malloc(o.l + 1);
	i = 0;
	y = 0;
	while (s.s[y])
	{
		o.s[i] = s.s[y];
		y += 1;
		i += 1;
	}
	y = 0;
	while (b.s[y])
	{
		o.s[i] = b.s[y];
		y += 1;
		i += 1;
	}
	o.s[i] = 0;
	return (o);
}

static t_string	string_sub(t_string s, ssize_t a, ssize_t l)
{
	ssize_t		i;
	ssize_t		y;
	t_string	o;

	o.l = l;
	o.s = malloc(l + 1);
	i = 0;
	y = 0;
	while (s.s[i])
	{
		if (i >= a && (i - a) < l)
		{
			o.s[y++] = s.s[i];
		}
		i += 1;
	}
	o.s[y] = 0;
	return (o);
}

static t_string	try_prev(int fd, t_fda *fda)
{
	t_string	o;
	t_string	s;
	ssize_t		i;

	i = 0;
	while (fda->prev[fd].s[i] != '\n' && fda->prev[fd].s[i])
		i += 1;
	if (fda->prev[fd].s[i] == '\n')
	{
		o = string_sub(fda->prev[fd], 0, i + 1);
		s = string_sub(fda->prev[fd], i + 1, fda->prev[fd].l - i - 1);
		free(fda->prev[fd].s);
		fda->prev[fd] = s;
		return (o);
	}
	return ((t_string){o.s = 0, o.l = 0});
}

static char	*try_read(int fd, t_fda *fda, ssize_t b)
{
	t_string	o;

	if (b < 0)
		return (0);
	if (b == 0)
	{
		if (fda->prev[fd].s && fda->prev[fd].s[0])
		{
			o = string_sub(fda->prev[fd], 0, fda->prev[fd].l);
			free(fda->prev[fd].s);
			fda->prev[fd].s = 0;
			return (o.s);
		}
		else
			return (0);
	}
	fda->buffer[b] = 0;
	o = string_join(fda->prev[fd], (t_string){.s = fda->buffer, .l = b});
	free(fda->prev[fd].s);
	fda->prev[fd] = o;
	return ((char *)42);
}

char	*get_next_line(int fd)
{
	static t_fda	fda;
	t_string		o;
	t_string		s;
	ssize_t			b;

	if (BUFFER_SIZE <= 0)
		return (0);
	if (!fda.prev[fd].s)
	{
		fda.prev[fd].s = malloc(1);
		*(fda.prev[fd].s) = 0;
	}
	while (1)
	{
		o = try_prev(fd, &fda);
		if (o.s)
			return (o.s);
		o.s = try_read(fd, &fda, read(fd, fda.buffer, BUFFER_SIZE));
		if (o.s != (char *)42)
			return (o.s);
	}
}
