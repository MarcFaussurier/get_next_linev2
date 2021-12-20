#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 80
# endif
# ifndef FD_MAX
#  define FD_MAX 256
# endif
typedef char *t_str;
typedef struct 
{
	t_str		s;
	size_t		l;
}				t_pstr;
typedef struct s_line
{
	char		*o;
	char		*n;
	size_t		x;
	size_t		y;
	size_t		z;
	int			f;
}				t_line;
typedef struct s_gnl
{
	char		buffer[BUFFER_SIZE + 1];
	char		*strs[FD_MAX];
	size_t 		sizes[FD_MAX];
}				t_gnl;
char			*get_next_line(int fd);

#endif
