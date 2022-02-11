#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

char	*get_next_line(int fd);

int	main(int ac, char **av)
{
	char		*o;
	int			fd;
	static int	i;

	if (ac != 2)
	{
		printf("usage: %s <file>\n", av[0]);
		return (0);
	}
	fd = open(av[1], O_RDONLY);
	while (1)
	{
		o = get_next_line(fd);
		if (!o)
			break ;
		printf("-- %s", o);
		free(o);
	}
	close(fd);
	return (0);
}
