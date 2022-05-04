#include "minishell.h"

int		gnl_jb(int fd, char **line)
{
	static char		buf[MAX_FD][BUFFER_SIZE + 1];
	char			*adr;

	if (BUFFER_SIZE < 1 || fd < 0 || !line ||
			fd > MAX_FD || read(fd, buf[fd], 0) == -1)
		return (-1);
	if (!(*line = ft_strnew(0)))
		return (0);
	while (!(adr = ft_strchr(buf[fd], '\n')))
	{
		if (!(join_newstr(line, buf[fd])))
			return (-1);
		ft_memset(buf[fd], 0, BUFFER_SIZE);
		if (!(read(fd, buf[fd], BUFFER_SIZE)))
		{
			if (**line)
				buf[fd][0] = 0;
			else
				return (0);
		}
	}
	*adr = 0;
	if (!(join_newstr(line, buf[fd])))
		return (-1);
	ft_strncpy(buf[fd], adr + 1, sizeof(buf[fd]));
	return (1);
}

void		handle_sigint(int sign)
{
	if (sign == SIGINT)
		ft_putstr("\nminishell-1.0$ ");
}

int			main(int ac, char *av[], char *ep[])
{
	char	*line;
	int		ret;

	signal(SIGINT, handle_sigint);
	ft_putstr("minishell-1.0$ ");
	line = NULL;
	while ((ret = gnl_jb(0, &line)) > 0)
	{
		printf("|%s|\n", line);
		free(line);
		line = NULL;
		ft_putstr("minishell-1.0$ ");
	}
	free(line);
	line = NULL;
	if (ret == -1)
		return (-1);
	if (ret == 0)
	{
		ft_putstr("exit\n");
	}
	//system("leaks testit");
	if (ac && av && ep)
		;
	return (0);
}
