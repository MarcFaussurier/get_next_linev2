# get_next_linev2


test it:
```shell
  cc -DBUFFER_SIZE=42 *.c && ./a.out bible.txt
```

customize BUFFER_SIZE=XX or FD_MAX=yy at compile time, here are the defaults values:
```C
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 80
# endif
# ifndef FD_MAX
#  define FD_MAX OPEN_MAX
# endif
```

This project exports two functions:
```C
char			*get_next_line(int fd);
char			*get_next_token(int fd, int (*issep)(char c) );
```

Here is what get_next_line.c looks like:
```C
#include "get_next_line.h"

static int 		ft_isnl(char c)
{
	return (c == '\n');
}

char			*get_next_line(int fd)
{
	return (get_next_token(fd, ft_isnl));
}
```

t_gnt is the only static variable used in get_next_token():
- char buffer[] used by read() is also static because virtual memory is larger than plain stack one
- char *strs[] is an fd array of ptrs for storing prev buffers for the multi-fd feature
- size_t sizes[] store previous buffer length to optimize iterations and memory
```C
typedef struct          s_gnt
{
	char		buffer[BUFFER_SIZE + 1];
	char		*strs[FD_MAX];
	size_t 		sizes[FD_MAX];
}			t_gnt;
```


