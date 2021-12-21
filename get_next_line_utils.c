#include "get_next_line.h"

static void* 		ft_buffer_append(char *input, char *outputs[2], size_t *its[2], int (*issep)(char c))
{
	size_t 			x;

	x = 0;
	while (input[x])
	{
		outputs[0][*(its[0])] = input[x++];
		if (issep)
		{
			outputs[1][(*(its[1]))++] = outputs[0][*(its[0])];
			if (issep(outputs[0][(*(its[0]))++]))
			{
				*(its[0]) = 0;
				issep = 0;
			}
		}

	}
	return (issep);
}

static void 		*ft_token_found(t_gnt *ctx, int fd, char *outputs[2], int (*issep)(char c))
{
	size_t			y;
	size_t			z;

	y = 0;
	z = 0;
	issep = ft_buffer_append(
			ctx->strs[fd], 
			outputs, 
			(size_t*[2]) {&y, &z},
			*issep);
	issep = ft_buffer_append(
			ctx->buffer, 
			outputs, 
			(size_t*[2]) {&y, &z},
			*issep);
	free(ctx->strs[fd]);
	outputs[0][y] = 0;
	outputs[1][z] = 0;
	ctx->strs[fd] = outputs[0];
	ctx->sizes[fd] = y;
	ctx->buffer[0] = 0;
	return (issep);
}

static char			*ft_try_read_token(t_gnt *ctx, int fd, ssize_t *bytes, int (*issep)(char c) )
{
	char 			*n;
	char 			*o;

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
	if (ft_token_found(ctx, fd, (char*[2]){n, o}, issep))
	{
		if (!(*bytes <= 0 && o[0]))
		{
			free(o);
			return 0;
		}
		n[0] = 0;
	}
	return (o);
}


#if BUFFER_SIZE < 1
char				*get_next_token(int fd, int (*issep)(char c))
{
	(void) fd;
	(void) issep;
	return (0);
}

#else
char				*get_next_token(int fd, int (*issep)(char c) )
{
	static t_gnt	ctx;
	ssize_t			bytes;
	char			*token;

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
		token = ft_try_read_token(&ctx, fd, &bytes, issep); 
		if (token)
			return (token);
	}
	free(ctx.strs[fd]);
	ctx.strs[fd] = 0;
	ctx.buffer[0] = 0;
	return (0);
}
#endif
