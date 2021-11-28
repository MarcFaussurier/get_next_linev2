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
typedef struct s_fdarray
{
	char		buffer[BUFFER_SIZE + 1];
	char		*strs[FD_MAX];
}				t_fdarray;
char			*get_next_line(int fd);

#endif
