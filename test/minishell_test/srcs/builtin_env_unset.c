#include "minishell.h"

int	env_builtin(t_list **env, t_command *cmd)
{
	if ((*env)->next == NULL)
		g_exit_status = 0;
	if ((*env)->next != NULL)
		env_builtin(&((*env)->next), cmd);
	ft_putstr_fd(((t_env *)((*env)->content))->key, cmd->fd[1]);
	ft_putstr_fd(((t_env *)((*env)->content))->value, cmd->fd[1]);
	ft_putchar_fd('\n', cmd->fd[1]);
	return (0);
}

static int	check_unset_arg(char *arg, t_command *cmd)
{
	int		ret;
	int		i;

	i = -1;
	ret = 1;
	while (arg[++i])
		if ((!ft_isalnum(arg[i]) && arg[i] != '_') || arg[i] == '=')
			ret = 0;
	if (ft_isdigit(*arg) || !ret || !*arg)
	{
		error_msg("bash", cmd, arg, "not a valid identifier");
		g_exit_status = 1;
		return (0);
	}
	return (1);
}

int	is_piped(int *fd)
{
	struct stat	fd_check;

	fstat(fd[1], &fd_check);
	if (S_ISFIFO(fd_check.st_mode))
		return (1);
	fstat(fd[0], &fd_check);
	if (S_ISFIFO(fd_check.st_mode))
		return (1);
	return (0);
}

static int	check_option(t_command *cmd)
{
	char	buf[3];

	ft_memset(buf, 0, sizeof(buf));
	if (cmd->command[1][0] == '-')
	{
		error_msg("bash", cmd, ft_strncpy(buf, cmd->command[1], 2),
			"invalid option");
		g_exit_status = 2;
		return (1);
	}
	return (0);
}

int	unset_builtin(t_list **env, t_command *cmd, t_list **export)
{
	int		piped;
	int		i;

	i = 0;
	update_underscore(env, last_arg(cmd));
	if (!cmd->command[0] || !cmd->command[1])
		return (RT_SUCCESS);
	if (check_option(cmd))
		return (RT_SUCCESS);
	piped = is_piped(cmd->fd);
	while (cmd->command[++i])
	{
		if (!check_unset_arg(cmd->command[i], cmd))
			continue ;
		if (piped)
			continue ;
		delete_env_variable(env, cmd->command[i]);
		delete_env_variable(export, cmd->command[i]);
		g_exit_status = 0;
	}
	return (RT_SUCCESS);
}
