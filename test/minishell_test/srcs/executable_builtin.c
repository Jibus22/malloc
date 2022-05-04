#include "minishell.h"

int	is_builtin(t_command *cmd)
{
	char	*name[8];
	int		builtin_code;
	int		i;

	if (!(cmd->command[0]))
		return (0);
	name[0] = "echo";
	name[1] = "pwd";
	name[2] = "exit";
	name[3] = "cd";
	name[4] = "export";
	name[5] = "unset";
	name[6] = "env";
	name[7] = NULL;
	builtin_code = 0;
	i = -1;
	while (name[++i])
		if (++builtin_code && !(ft_strcmp(cmd->command[0], name[i])))
			return (builtin_code);
	return (0);
}

int	execute_builtin(t_list **env, t_command *cmd, int builtin_code,
						t_list **export)
{
	int		ret;

	if (builtin_code == BT_ECHO)
		ret = echo_builtin(env, cmd);
	else if (builtin_code == BT_PWD)
		ret = pwd_builtin(env, cmd);
	else if (builtin_code == BT_EXIT)
		ret = exit_builtin(cmd);
	else if (builtin_code == BT_CD)
		ret = cd_builtin(env, cmd);
	else if (builtin_code == BT_EXPORT)
		ret = export_builtin(env, cmd, export);
	else if (builtin_code == BT_UNSET)
		ret = unset_builtin(env, cmd, export);
	else
	{
		update_underscore(env, "env");
		ret = env_builtin(env, cmd);
	}
	if (ret == RT_EXIT)
		return (RT_EXIT);
	if (ret == RT_NOEXIT)
		return (RT_NOEXIT);
	return (RT_SUCCESS);
}
