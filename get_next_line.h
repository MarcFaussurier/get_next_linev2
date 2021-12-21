#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 80
# endif
# ifndef FD_MAX
#  define FD_MAX OPEN_MAX
# endif
typedef struct s_gnt
{
	char		buffer[BUFFER_SIZE + 1];
	char		*strs[FD_MAX];
	size_t 		sizes[FD_MAX];
}				t_gnt;
char			*get_next_line(int fd);
char			*get_next_token(int fd, int (*issep)(char c) );
#endif
