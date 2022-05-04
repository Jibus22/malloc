#include "minishell.h"

static void	update_export_underscore(t_list **env, t_command *cmd)
{
	char	*last;
	char	*needle;
	int		len;

	last = last_arg(cmd);
	needle = ft_strchr(last, '=');
	if (!(needle))
		len = ft_strlen(last);
	else
		len = needle - last;
	needle = ft_substr(last, 0, len);
	if (!(needle))
		return ;
	update_underscore(env, needle);
	free(needle);
}

static int	check_export_arg(char *arg, t_command *cmd)
{
	int		ret;
	int		end;
	int		i;

	end = ft_strclen(arg, '=');
	i = -1;
	ret = 1;
	while (arg[++i] && i < end)
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			ret = 0;
	if (ft_isdigit(*arg) || !ret || !end)
	{
		error_msg("bash", cmd, arg, "not a valid identifier");
		g_exit_status = 1;
		return (0);
	}
	return (1);
}

static int	export_builtin_arg(t_list **env, t_list **export,
								t_command *cmd, int piped)
{
	int		i;

	i = 0;
	while (cmd->command[++i])
	{
		if (!check_export_arg(cmd->command[i], cmd))
			continue ;
		if (piped)
			continue ;
		g_exit_status = 0;
		if (ft_strchr(&cmd->command[i][0], '=') != NULL)
		{
			add_env_variable(env, &cmd->command[i][0]);
			add_env_variable(export, &cmd->command[i][0]);
		}
		else if (ft_strchr(&cmd->command[i][0], '=') == NULL)
			add_env_variable(export, &cmd->command[i][0]);
	}
	return (RT_SUCCESS);
}

int	export_option_err_mess(t_command *cmd)
{
	char	buf[3];

	ft_memset(buf, 0, sizeof(buf));
	error_msg("bash", cmd, ft_strncpy(buf, cmd->command[1], 2),
		"invalid option");
	g_exit_status = 2;
	return (RT_SUCCESS);
}

int	export_builtin(t_list **env, t_command *cmd, t_list **export)
{
	char	**export_tab;
	int		piped;

	update_export_underscore(env, cmd);
	if (cmd->command[0] && !cmd->command[1])
	{
		export_tab = env_list_to_tab(*export);
		if (!(export_tab))
			return (RT_FAIL);
		print_export(export_tab, cmd->fd);
		ft_freetab(export_tab);
		return (RT_SUCCESS);
	}
	if (cmd->command[1] && cmd->command[1][0] == '-')
		return (export_option_err_mess(cmd));
	piped = is_piped(cmd->fd);
	if (cmd->command[0] && cmd->command[1])
		export_builtin_arg(env, export, cmd, piped);
	return (RT_SUCCESS);
}
