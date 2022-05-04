#include "minishell.h"

void	dup_fd(int *fd)
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

void	close_fd(int *fd)
{
	if (fd[1] != 1 && fd[1] != -1)
		close(fd[1]);
	if (fd[0] != 0 && fd[0] != -1)
		close(fd[0]);
}
