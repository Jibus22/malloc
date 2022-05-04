#include "minishell.h"

/*
** TESTING PRINT
*/

void	print_cmd(t_command *cmd)
{
	int	i;

	printf("file : %s\t\t\tfd[0] : %d\tfd[1] : %d\n",
		cmd->file, cmd->fd[0], cmd->fd[1]);
	i = -1;
	while (cmd->command[++i])
		printf("cmd->command[%d] = |%s|\n", i, cmd->command[i]);
	printf("cmd->command[%d] = |%s|\n", i, cmd->command[i]);
	fflush(stdout);
}

void	print_array(char **arr)
{
	int	i;

	printf("###printarray####\n");
	i = -1;
	while (arr[++i])
		printf("str[%d] : |%s|\n", i, arr[i]);
	printf("str[%d] : |%s|\n", i, arr[i]);
	fflush(stdout);
}
