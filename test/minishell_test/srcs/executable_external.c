#include "minishell.h"

/*
** waitpid will wait until child process exits.
** Here the child fork pid is returned to the parent.
**
** setting up error:
** WIFEXITED(wstatus) - macro that returns true if the child terminated normally
** WEXITSTATUS(wstatus) - macro that returns the exit status of the child
** WIFSIGNALED(wstatus) - macro that returns true if the child process was
** terminated by a signal
** WTERMSIG(wstatus) - macro that returns the number of the signal that caused
** the child process to terminate
**
** waiting for a SIGQUIT (ctrl\) signal if any, while child does not quit (if
** it gets to quit).
*/
static int	parent_process(pid_t pid, char *pathcmd, char **env_tab)
{
	int		wstatus;

	free(pathcmd);
	ft_freetab(env_tab);
	wstatus = 0;
	waitpid(pid, &wstatus, 0);
	if (WIFEXITED(wstatus))
		g_exit_status = WEXITSTATUS(wstatus);
	else if (WIFSIGNALED(wstatus))
		g_exit_status = WTERMSIG(wstatus);
	signal(SIGQUIT, ctrl_back_slash_handler);
	if (g_exit_status == 3)
		g_exit_status = 131;
	return (RT_SUCCESS);
}

/*
** Here, enter has already been precessed (so ret_gnl = 1). So it'll wait for
** a SIGQUIT (ctrl\) and Quit (core dumped) if so.
**
** Fork - creates a new process (child) from the calling process (parent).
**
** On success:
**		the PID of the child process is returned in the parent, and 0 is
** returned in the child.
**		child process fork will execute things here.
**		Execve will receive only absolute path executables, that will be either
**		input by the user, or transformed into an aboslute path executable by
**		the functions inside the file executable_builtin_path.c
**		If execve fails, it outputs an error message.
**		exit 127 to exit the child process
**
**		OBS.: I think that as "the child process and the parent process run in
**		separate memory spaces"), the memory is also duplicated, therefore it
**		needs to be freed inside the child too (and then also on the parent).
**
** On failure:
**		-1 is returned in  the parent, no child process is created, and
** error number is set appropriately inside the function parent_process().
*/

static int	return_fork_error(char *path_to_cmd, char **env_tab)
{
	free(path_to_cmd);
	ft_freetab(env_tab);
	g_exit_status = 2;
	return (RT_SUCCESS);
}

static int	fork_extern(t_command *cmd, char *path_to_cmd, char **env_tab)
{
	int	cpid;

	cpid = fork();
	if (cpid == -1)
		return (return_fork_error(path_to_cmd, env_tab));
	else if (cpid == 0)
	{
		dup_fd(cmd->fd);
		if (execve(path_to_cmd, cmd->command, env_tab) == -1)
		{
			error_msg("bash", cmd, NULL, strerror(errno));
			free(path_to_cmd);
			ft_freetab(env_tab);
		}
		if (errno == EACCES)
			g_exit_status = 126;
		else if (errno == ENOENT)
			g_exit_status = 127;
		return (RT_FAIL);
	}
	else
		return (cpid);
}

static int	check_special_case(t_command *cmd)
{
	if (!(cmd->command[0]) || !(cmd->command[0][0]))
	{
		g_exit_status = 0;
		return (RT_SUCCESS);
	}
	return (1);
}

int	execute_extern(t_list **env, t_command *cmd, char *saved_path)
{
	char	**env_tab;
	char	*path_to_cmd;
	pid_t	fork_pid;

	if (check_special_case(cmd) == RT_SUCCESS)
		return (RT_SUCCESS);
	path_to_cmd = path_to_executable(env, cmd, saved_path);
	if (!(path_to_cmd))
		return (RT_FAIL);
	if (!*path_to_cmd)
	{
		g_exit_status = 127;
		return (RT_SUCCESS);
	}
	signal(SIGQUIT, ctrl_back_slash_handler_quit);
	env_tab = env_list_to_tab(*env);
	if (!(env_tab))
		return (RT_FAIL);
	update_underscore(env, last_arg(cmd));
	fork_pid = fork_extern(cmd, path_to_cmd, env_tab);
	if (fork_pid == RT_FAIL)
		return (RT_FAIL);
	return (parent_process(fork_pid, path_to_cmd, env_tab));
}
