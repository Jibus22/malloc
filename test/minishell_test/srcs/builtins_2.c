#include "minishell.h"

int	pwd_builtin(t_list **env, t_command *cmd)
{
	char		*stored;
	char		*tmp;

	update_underscore(env, last_arg(cmd));
	g_exit_status = 0;
	stored = getcwd(NULL, 0);
	tmp = find_env_value(env, "PWD");
	if (tmp && ft_strcmp(tmp, "//") == 0)
		ft_putstr_fd("//", cmd->fd[1]);
	else
		ft_putstr_fd(stored, cmd->fd[1]);
	ft_putchar_fd('\n', cmd->fd[1]);
	free(stored);
	return (0);
}

/*
** If the index equals the length of the first argument, it means it is and
** intire numerical argument, so we can check the second argument, if any.
**
** 1. Checking if there is no second argument, if so, multiplies and module
** divides the errno so we can have a correct exit status (and exit minishell).
** 2. Otherwise, if there is a 2nd parameter, output the correct error message.
*/

static int	exit_arg(t_command *cmd, size_t i)
{
	if (i == ft_strlen(cmd->command[1]))
	{
		if (!cmd->command[2])
		{
			g_exit_status = ft_atoi(cmd->command[1]);
			g_exit_status += 256;
			g_exit_status %= 256;
			return (RT_EXIT);
		}
		else if (cmd->command[2])
		{
			error_msg("bash", cmd, NULL, "too many arguments");
			g_exit_status = 1;
			return (RT_NOEXIT);
		}
	}
	return (RT_EXIT);
}

/*
** exit <nb less than long long>
** output: exit\n
** echo $? = if nb > 255 < longlong, it starts over again to count from 0 to 255
**
** exit <any string or number bigger than a long long>
** output: exit\nbash: exit: <argv>: numeric argument required\n
** echo $? = 2
**
** exit <str or nb less than long long> <str or nb less than long long>
** output: exit\nbash: exit: too many arguments\n
** IT DOES EXIT THE SHELL
** echo $? = 1
**
** If <argv> is omitted, the exit status is that of the last command executed.
**
**
** If there is one argument:
**		1. if it's a + or -, increase the endex (ignore it).
**		2. while the argument is a number, increase the index.
**		3. if the index is different form the length of the first argument, then
**		it means that this argument does not contains only numberm an so, it's
**		not a intire numeric argument. Then, output the correct error messsage.
*/

int	exit_builtin(t_command *cmd)
{
	size_t		i;
	long long	ret_atoi;

	i = 0;
	ft_putstr_fd("exit\n", 2);
	if (cmd->command[1] == NULL)
		return (RT_EXIT);
	else if (cmd->command[1])
	{
		ret_atoi = ft_long_long_atoi(cmd->command[1]);
		if (cmd->command[1][0] == '+' || cmd->command[1][0] == '-')
			i++;
		while (ft_isdigit((char)cmd->command[1][i]) == 1)
			i++;
		if (i != ft_strlen(cmd->command[1]) || (ret_atoi == -1
				&& ft_strcmp(cmd->command[1], "-1") != 0))
		{
			error_msg("y", cmd, cmd->command[1], "numeric argument required");
			g_exit_status = 2;
			return (RT_EXIT);
		}
		if (exit_arg(cmd, i) == RT_NOEXIT)
			return (RT_NOEXIT);
	}
	return (RT_EXIT);
}
