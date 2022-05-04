#include "minishell.h"

static void	print_getcwd_err_message(void)
{
	ft_putstr_fd("error retrieving current directory: getcwd: cannot access\
 parent directories: ", 2);
	ft_putstr_fd(strerror(errno), 2);
}

int	cd_error(int err, char *path)
{
	g_exit_status = 1;
	if (err == CD_FIFO)
		return (0);
	if (err < GETCWD_ERR)
		ft_putstr_fd("bash: ", 2);
	ft_putstr_fd("cd: ", 2);
	if (err == HOME_NOT_SET)
		ft_putstr_fd("HOME not set", 2);
	else if (err == OLDPWD_NOT_SET)
		ft_putstr_fd("OLDPWD not set", 2);
	else if (err == INVAL_CD_OPTION)
		ft_putstr_fd("invalid option\ncd: usage: cd [-L|-P] [dir]", 2);
	else if (err == TOO_MANY_ARGS)
		ft_putstr_fd("too many arguments", 2);
	else if (err == ERRNO_CD)
	{
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
	}
	else if (err == GETCWD_ERR)
		print_getcwd_err_message();
	ft_putchar_fd('\n', 2);
	return (0);
}
