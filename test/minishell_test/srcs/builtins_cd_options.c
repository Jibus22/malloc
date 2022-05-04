#include "minishell.h"

static char	*get_old(t_list **env, int *err)
{
	char	*old;

	old = find_env_value(env, "OLDPWD");
	if (!old)
		*err = OLDPWD_NOT_SET;
	return (old);
}

static char	*get_home(t_list **env, int *err)
{
	char	*home;

	home = find_env_value(env, "HOME");
	if (!home)
		*err = HOME_NOT_SET;
	return (home);
}

static char	*home_option(t_list **env, char **cmd, int *err)
{
	char	*path;

	path = NULL;
	if (cmd[1][2])
		*err = INVAL_CD_OPTION;
	else
		path = get_home(env, err);
	return (path);
}

static char	*old_option(t_list **env, char **cmd, int *fd, int *err)
{
	char	*path;

	path = NULL;
	if (cmd[1][1])
		*err = INVAL_CD_OPTION;
	else
	{
		path = get_old(env, err);
		if (!path)
			return (path);
		ft_putstr_fd(path, fd[1]);
		ft_putchar_fd('\n', fd[1]);
	}
	return (path);
}

char	*check_options(t_list **env, char **cmd, int *fd, int *err)
{
	char	*path;

	*err = 0;
	path = NULL;
	if (!cmd[1])
		path = get_home(env, err);
	else if (!ft_strncmp(cmd[1], "--", 2))
		path = home_option(env, cmd, err);
	else if (!ft_strncmp(cmd[1], "-", 1))
		path = old_option(env, cmd, fd, err);
	if (cmd[1])
		if (cmd[2])
			*err = TOO_MANY_ARGS;
	return (path);
}
