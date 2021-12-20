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

static char		*ft_token(t_gnl *ctx, int fd, ssize_t *bytes, int (*issep)(char c) )

{
	char		*o;
	char		*n;
	size_t		x;
	size_t		y;
	size_t		z;
	int			f;

	if (*bytes < 0)
		return (0);
	ctx->buffer[*bytes] = 0;
	n = malloc (ctx->sizes[fd] + BUFFER_SIZE + 1);
	o = malloc (ctx->sizes[fd] + BUFFER_SIZE + 1);
	if (!n || !o)
	{
		free(n);
		free(o);
		*bytes = -1;
		return (0);

	}

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
		if (issep(n[y++]) && !f)
		{
			y = 0;
			f = 1;
		}
	}
	n[y] = 0;
	

	o[z] = 0;
	free(ctx->strs[fd]);
	ctx->strs[fd] = n;
	ctx->sizes[fd] = z;
	ctx->buffer[0] = 0;

	if (!f)
	{
		if (!(*bytes <= 0 && o[0]))
		{
			free(o);
			return 0;
		}
		n[0] = 0;
	}
	return o;
}




#if BUFFER_SIZE < 1

char			*get_next_token(int fd)
{
	(void)fd;
	return (0);
}

#else

char			*get_next_token(int fd, int (*issep)(char c) )
{
	static t_gnl		ctx;
	ssize_t				bytes;
	char				*line;

	if (fd < 0 || fd > FD_MAX)
		return (0);
	if (!ctx.strs[fd])
	{
		ctx.strs[fd] = malloc(1);
		ctx.strs[fd][0] = 0;
		ctx.sizes[fd] = 0;
	}
	bytes = 1;
	while (bytes > 0)
	{
		bytes = read(fd, ctx.buffer, BUFFER_SIZE);
		line = ft_token(&ctx, fd, &bytes, issep); // may be LAST(NO NL), LINE, LAST(NL)
		if (line)
			return (line);
	}
	free(ctx.strs[fd]);
	ctx.strs[fd] = 0;
	ctx.buffer[0] = 0;
	return (0);
}

static int 		ft_isnl(char c)
{
	return (c == '\n');
}

char			*get_next_line(int fd)
{
	return (get_next_token(fd, ft_isnl));
}
#endif
