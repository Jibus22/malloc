#include "minishell.h"

static int	update_pwd(t_list **env, char *cmd_arg)
{
	char	*tmp;
	char	*pwd;
	char	*old_pwd;

	old_pwd = find_env_value(env, "PWD");
	tmp = ft_strjoin("OLDPWD=", old_pwd);
	if (!(tmp))
		return (RT_FAIL);
	add_env_variable(env, tmp);
	free(tmp);
	pwd = getcwd(NULL, 0);
	if (!(pwd))
		return (GETCWD_ERR);
	tmp = ft_strjoin("PWD=", pwd);
	if (!(tmp))
		return (RT_FAIL);
	if (cmd_arg && ft_strcmp(cmd_arg, "//") == 0)
	{
		free(tmp);
		tmp = ft_strdup("PWD=//");
	}
	add_env_variable(env, tmp);
	free(tmp);
	free(pwd);
	return (RT_SUCCESS);
}

static int	check_fd_n_path(char *path, t_command *cmd, int *err)
{
	struct stat	fd_check;
	struct stat	path_check;
	int			ret;

	ret = stat(path, &path_check);
	if (!ret && !(S_ISDIR(path_check.st_mode)))
	{
		*err = ERRNO_CD;
		return (1);
	}
	ret = fstat(cmd->fd[1], &fd_check);
	if (!ret && S_ISFIFO(fd_check.st_mode))
	{
		*err = CD_FIFO;
		return (1);
	}
	ret = fstat(cmd->fd[0], &fd_check);
	if (!ret && S_ISFIFO(fd_check.st_mode))
	{
		*err = CD_FIFO;
		return (1);
	}
	return (0);
}

static int	change_directory_n_update_env(t_list **env, t_command *cmd,
											char *path)
{
	int	err;

	if (chdir(path) == -1)
	{
		err = ERRNO_CD;
		return (cd_error(err, path));
	}
	err = update_pwd(env, cmd->command[1]);
	if (err == RT_FAIL)
		return (RT_FAIL);
	if (err)
		return (cd_error(err, path));
	g_exit_status = 0;
	return (0);
}

int	cd_builtin(t_list **env, t_command *cmd)
{
	char	*path;
	char	cwd[300];
	int		err;

	update_underscore(env, last_arg(cmd));
	path = check_options(env, cmd->command, cmd->fd, &err);
	if (err)
		return (cd_error(err, cmd->command[1]));
	if (!path)
		path = cmd->command[1];
	ft_memset(cwd, 0, sizeof(cwd));
	if (!*path)
	{
		path = getcwd(cwd, 300);
		if (!path && (err == GETCWD_ERR))
			return (cd_error(err, cmd->command[1]));
	}
	if (check_fd_n_path(path, cmd, &err))
		return (cd_error(err, path));
	change_directory_n_update_env(env, cmd, path);
	return (0);
}
