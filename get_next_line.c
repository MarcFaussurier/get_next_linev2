/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   get_next_line.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   Authors: marvin                                +:+   +:    +:    +:+     */
/*   <marvin@42lyon.fr>                            #+#   #+    #+    #+#      */
/*                                                #+#   ##    ##    #+#       */
/*                                               ###    #+./ #+    ###.fr     */
/*                                                        /   UNIV -          */
/*                                               | |  _  / ___ _ _   / |      */
/*   Created: 2021/11/29 14:39:13 by marvin      | |_| || / _ \ ' \  | |      */
/*   Updated: 2021/12/08 16:59:41 by marvin      |____\_, \___/_||_| |_|      */
/*                                                    /__/            .fr     */
/* ************************************************************************** */

#include "get_next_line.h"
#include "string.h"
#include <stdio.h>
static void		ft_do(char *str, t_pstr *a, t_pstr *b, int *f)
{
	x = 0;
	while (str[x])
	{
		a->s[a->l] = str[x++];
		if (!*f)
			b->s[b->l++] = a->s[a->l];
		if (a->s[a->l++] == '\n' && !*f)
		{
			a->l = 0;
			*f = 1;
		}
	}
}

static char		*ft_line(t_gnl *ctx, int fd, ssize_t bytes)
{
	char		*o;
	char		*n;
	size_t		x;
	size_t		y;
	size_t		z;
	int			f;

	//printf("[%s][%s]\n", ctx->strs[fd], ctx->buffer);
	x = 0;
	while (ctx->strs[fd][x++]);
	n = malloc (x + BUFFER_SIZE + 1);
	o = malloc (x + BUFFER_SIZE + 1);
	x = 0;
	y = 0;
	z = 0;
	f = 0;
	while (ctx->strs[fd][x])
	{
		n[y] = ctx->strs[fd][x++];
		if (!f)
			o[z++] = n[y];
		if (n[y++] == '\n' && !f)
		{
			y = 0;
			f = 1;
		}
	}
	x = 0;
	while (ctx->buffer[x])
	{
		n[y] = ctx->buffer[x++];
		if (!f)
			o[z++] = n[y];
		if (n[y++] == '\n' && !f)
		{
			y = 0;
			f = 1;
		}
	}
	n[y] = 0;
	o[z] = 0;
	free(ctx->strs[fd]);
	ctx->strs[fd] = n;
	ctx->buffer[0] = 0;

	if (!f)
	{
		if (!(bytes <= 0 && o[0]))
		{
			free(o);
			return 0;
		}
		n[0] = 0;
	}
	return o;
}

#if BUFFER_SIZE < 1

char			*get_next_line(int fd)
{
	(void)fd;
	return (0);
}

#else

char			*get_next_line(int fd)
{
	static t_gnl		ctx;
	ssize_t				bytes;
	char				*line;

	if (fd < 0)
		return (0);
	if (!ctx.strs[fd])
	{
		ctx.strs[fd] = malloc(1);
		ctx.strs[fd][0] = 0;
	}
	bytes = 1;
	while (bytes > 0)
	{
		bytes = read(fd, ctx.buffer, BUFFER_SIZE);
		if (bytes >= 0)
			ctx.buffer[bytes] = 0;
		line = ft_line(&ctx, fd, bytes); // may be LAST(NO NL), LINE, LAST(NL)
		if (line)
			return (line);
	}
	free(ctx.strs[fd]);
	ctx.strs[fd] = 0;
	ctx.buffer[0] = 0;
	return (0);
}
#endif
