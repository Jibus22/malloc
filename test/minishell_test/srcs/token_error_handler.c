#include "minishell.h"

int	tokenize_error_pipe(t_list **head, char **pipeline,
									int i, int fd_tmp)
{
	if (fd_tmp != -1)
		close(fd_tmp);
	while (pipeline[i])
		free(pipeline[i++]);
	free(pipeline);
	if (head)
		ft_lstclear(head, &clear_commandlist);
	return (RT_FAIL);
}

void	*tokenize_error_sc(t_list **head, char **array, char *line)
{
	ft_freetab(array);
	if (head)
		ft_lstclear(head, &clear_commandlist);
	free(line);
	return (NULL);
}

int	return_expander(t_list **arg, int ret, t_command *cmd)
{
	if (ret == RT_FAIL)
		return (RT_FAIL);
	g_exit_status = 1;
	error_msg_2("y", cmd, cmd->file, strerror(cmd->fd[3]));
	ft_lstclear(arg, &clear_arglist_2);
	return (RT_BAD_FD);
}
