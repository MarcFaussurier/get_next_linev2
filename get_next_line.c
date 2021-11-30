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
/*   Updated: 2021/11/30 13:04:20 by marvin      |____\_, \___/_||_| |_|      */
/*                                                    /__/            .fr     */
/* ************************************************************************** */

#include "get_next_line.h"
#include "string.h"
#include <stdio.h>
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
		if (bytes <= 0)
		{
			free(ctx->strs[fd]);
			ctx->strs[fd] = 0;
			if (o[0])
				return o;
		}
		free(o);
		return 0;
	}
	return o;
}

static char		*ft_exit(t_gnl *ctx, int fd, char *output)
{
	if (!output)
	{
		free(ctx->strs[fd]);
		ctx->strs[fd] = 0;
		ctx->buffer[0] = 0;
	}
	return (output);
}

char			*get_next_line(int fd)
{
	static t_gnl		ctx;
	ssize_t				bytes;
	char				*line;

	if (fd < 0 || fd > FD_MAX || BUFFER_SIZE < 1 || read(fd, 0, 0) < 0)
		return (0);
	if (!ctx.strs[fd])
	{
		ctx.strs[fd] = malloc(1);
		ctx.strs[fd][0] = 0;
	}
	bytes = 1;
	while (1)
	{
		line = ft_line(&ctx, fd, bytes); // may be LAST(NO NL), LINE, LAST(NL)
		if (!bytes || line)
			return (ft_exit(&ctx, fd, line));
		else 
			free(line);
		bytes = read(fd, ctx.buffer, BUFFER_SIZE);
		if (bytes >= 0)
			ctx.buffer[bytes] = 0;
	}
	return (ft_exit(&ctx, fd, 0));
}
