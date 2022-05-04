#include "minishell.h"

void		dup_it(int *fd)
{
	if (fd[0] != 0)
	{
		dup2(fd[0], 0);
		close(fd[0]);
	}
	if (fd[1] != 1)
	{
		dup2(fd[1], 1);
		close(fd[1]);
	}
}

void		clean_fd_n_wait(int *fd, int cpid)
{
	int		status;

	if (fd[1] != 1)
		close(fd[1]);
	if (fd[0] != 0)
		close(fd[0]);
	waitpid(cpid, &status, 0);
}

int			find_exec(char *path, char *bin_name)
{
	int				cmp;
	DIR				*dp;
	struct dirent	*dirp;

	if (!(dp = opendir(path)))
		return (-1);
	while ((dirp = readdir(dp)))
		if (!(cmp = ft_strcmp(bin_name, dirp->d_name)))
			break;
	closedir(dp);
	if (cmp == 0)
		return (0);
	return (-1);
}

char		*expand_path(t_list **env, char *arg_zero)
{
	int		i;
	char	*path;
	char	**paths;

	path = find_env_value(env, "PATH");
	if (!(paths = ft_split_jb(path, ':')))
		return (NULL);
	path = NULL;
	i = -1;
	while (paths[++i])
		if (!find_exec(paths[i], arg_zero))
			break;
	if (!paths[i])
	{
		printf("command not found\n");
		ft_freetab(paths);
		return (NULL);
	}
	if (!(path = ft_strjoin(paths[i], "/")))
		return (NULL);
	if (!(join_newstr(&path, arg_zero)))
		return (NULL);
	ft_freetab(paths);
	return (path);
}

char		*expand_tilde2(t_list **env, char *arg_zero)
{
	char	*home;
	char	*expanded;

	if (*(arg_zero + 1) != '/')
		return (NULL);
	home = find_env_value(env, "HOME");
	if (!(expanded = ft_strjoin(home, (arg_zero + 1))))
		return (NULL);
	return (expanded);
}

char		*find_path(t_list **env, char *arg_zero)
{
	char	*path;

	if (!*arg_zero || !*(arg_zero + 1))
		return (NULL);
	if (*arg_zero == '.' || *arg_zero == '/')
		path = ft_strdup(arg_zero);
	else if (*arg_zero == '~')
		path = expand_tilde2(env, arg_zero);
	else
		path = expand_path(env, arg_zero);
	if (!path)
		return (NULL);
	return (path);
}

int			exec_t(t_list **env, t_command *cmd, char **arr_env)
{
	char	*path;
	int		cpid;

	if (!cmd->command || !cmd->command[0])
		return (-1);
	if (cmd->fd[3] != 0)
		return (error_msg_2("y", cmd, cmd->file, strerror(cmd->fd[3])));
	if (!(path = find_path(env, cmd->command[0])))
		return (-1);
	if ((cpid = fork()) == -1)
		return (-1);
	if (cpid == 0)
	{
		dup_it(cmd->fd);
		if (execve(path, cmd->command, arr_env) == -1)
			printf("%s\n", strerror(errno));
		exit(127);
	}
	clean_fd_n_wait(cmd->fd, cpid);
	free(path);
	return (0);
}
