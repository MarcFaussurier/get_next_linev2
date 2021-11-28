#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(int ac, char **av)
{
	int		i;
	int		fd;
	char	*str;

	i = 1;
	while (i < ac)
	{
		fd = open (av[i], O_RDONLY);
		while (1)
		{
			str = get_next_line(fd);
			if (!str)
				break ;
			printf("- %s", str);
		}
		i += 1;
	}
}
