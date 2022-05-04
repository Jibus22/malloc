#include "minishell.h"

static int	echo_n_parser(char *str)
{
	if (!*str || *str != '-')
		return (1);
	str++;
	if (*str != 'n')
		return (1);
	while (*++str)
	{
		if (*str != 'n')
			return (1);
	}
	return (0);
}

static int	print_newline(t_command *cmd)
{
	ft_putchar_fd('\n', cmd->fd[1]);
	return (RT_SUCCESS);
}

int	echo_builtin(t_list **env, t_command *cmd)
{
	int			flag;
	int			i;

	update_underscore(env, last_arg(cmd));
	g_exit_status = 0;
	if (!cmd->command[1])
		return (print_newline(cmd));
	flag = 0;
	i = 1;
	while (cmd->command[i] && !echo_n_parser(cmd->command[i]))
	{
		flag = 1;
		i++;
	}
	while (cmd->command[i])
	{
		ft_putstr_fd(cmd->command[i], cmd->fd[1]);
		if (cmd->command[i + 1])
			ft_putchar_fd(' ', cmd->fd[1]);
		i++;
	}
	if (!flag)
		ft_putchar_fd('\n', cmd->fd[1]);
	return (RT_SUCCESS);
}
