#include "minishell.h"

static int	*init_fd(void)
{
	int		*fd;

	fd = malloc(sizeof(int) * 4);
	bzero(fd, sizeof(int) * 4);
	fd[0] = 0;
	fd[1] = 1;
	fd[2] = 2;
	return (fd);
}
void	push_to_list(char **cmd, t_list **list, int *fd)
{
	t_command *command;
	t_list		*tmp;

	tmp = *list;
	command = malloc(sizeof(t_command) + 1);
	command->fd = fd;
	command->command = cmd;
	if (!tmp)
		*list = ft_lstnew(command, sizeof(t_command));
	else {
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = ft_lstnew(command, sizeof(t_command));
	}
}

int		exec_cmd(t_command *cmd)
{
	pid_t	pid;

	if ((pid = fork()) == 0)
	{
		if (cmd->fd[0] != 0)
		{
			dup2(cmd->fd[0], 0);
			close(cmd->fd[0]);
		}
		if (cmd->fd[1] != 1)
		{
			dup2(cmd->fd[1], 1);
			close(cmd->fd[1]);
		}
		return execvp(cmd->command[0], cmd->command);
	}
	close(cmd->fd[1]);
	return pid;
}

//printf("command enter child with value: %s with fd[0] = %d and fd[1] = %d \n", cmd->command[0], cmd->fd[0], cmd->fd[1]);

int		main()
{
	t_list	*list;
	int pip[2];
	int *fd;
	int save = 0;
	char *ls[] = {"ls", "-la", 0};
	char *wc[] = {"wc", "-c", 0};
	char *sort[] = {"sort", 0};
	char	**cmd[] = {ls, sort, wc, NULL};
	int i = 0;

	list = NULL;
	while (cmd[i] != NULL)
	{
		fd = init_fd();
		pipe(pip);
		fd[0] = save;
		save = pip[0];
		if (cmd[i + 1] != NULL)
			fd[1] = pip[1];
		else
			close(pip[1]);
		printf("command enter list: %s with fd[0] = %d and fd[1] = %d \n", *cmd[i], fd[0], fd[1]);
		push_to_list(cmd[i], &list, fd);
		i++;
	}
	while (list->next != NULL)
	{
		printf("command enter exec: %s with fd[0]: %d and fd[1]:%d \n", ((t_command *)list->content)->command[0], ((t_command *)list->content)->fd[0], ((t_command *)list->content)->fd[1]);
		exec_cmd((t_command *)list->content);
		list = list->next;
	}
	if (((t_command *)list->content)->fd[0] != 0)
	{
		dup2(((t_command *)list->content)->fd[0], 0);
	}
	return execvp(((t_command *)list->content)->command[0], ((t_command *)list->content)->command);
}

/*
   int main(int ac, char *av[], char **ep)
   {
   if (ac > 1)
   printf("%s", *av);
   t_list    *list;
   list = create_env_list(ep);
   t_command *cmd;
   int			i = 0;

   cmd = malloc(sizeof(t_command));
   cmd->command = NULL;
   cmd->command = malloc(sizeof(char *) * 6);
   bzero(cmd->command, (sizeof(char *) * 6));
   while (i < 3)
   {
   cmd->fd[i] = i;
   i++;
   }

   cmd->command[0] = "tata";

   env_builtin(&list, cmd);
   cmd->command[1] = "TEST=TOTO";
   export_builtin(&list, cmd);
   cmd->command[1] = "TEST=RITOTO";
   export_builtin(&list, cmd);
   cmd->command[1] = "TATA=tutu";
   export_builtin(&list, cmd);
   cmd->command[1] = "TITI=tutu";
   export_builtin(&list, cmd);


   env_builtin(&list, cmd);

   ft_lstdel(&list, free_env);
   cmd->command[0] = "-nnnnnn";
   cmd->command[1] = "-n";
   cmd->command[2] = "nice";
   cmd->command[3] = "yop";
   cmd->command[4] = "lulz";
   echo_builtin(NULL, cmd);
   cmd->command -= 5;
   cmd->command[0] = "n";
   echo_builtin(NULL, cmd);
   return(0);
   }
   */
