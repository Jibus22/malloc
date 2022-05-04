#include "minishell.h"

/* Signal
**
** If the disposition is set to SIG_DFL, then the default action associated
** with the signal (see signal(7)) occurs.
**
**
** ctrl-C
** As ctrlC will just print ^C and go to the new line AT ANYTIME, we can handle
** it inside the loop for when:
** 1. a blocking command is typed, for example: grep h
** 2. if something is typed and ctrlD is typed before the return button, it
** writes ^C and goes to new line/promt.
** 3. for when command line is empty and no return button pressed (crtlC is
** pressed instead).
**
** ctrl-\
** Before the user input something, if ctrl\ is typed, nothing happpens inside
** the loop, if ctrl\ is typed, it has a different comportament).
** 1. if a blocking command is typed, ctrl\ kils the comman with a (core dumped)
** message and goes to a new line - i.e.: shows the prompt).
** 2. if something is typed and ctrl\ is typer before the return button, it
** does nothing.
*/

/* ctrl-D
** Before the user input something, if ctrlD is typed, the string "exit" is
** written after the prompt, then the shell closes, and $SHLVL environment
** variable is decreased after any input, if ctrlD is typed, it has a different
** comportament:
** 1. if a blocking command is typed, ctrlD stops the command and goes to a
** new line - i.e.: shows the prompt).
** 2. if something is typed and ctrlD is typed before the return button, it
** does nothing.
*/

/* This loop should only exit on ctrlD (sigquit) and "exit" builtin.
** It should be able to refuse to exit on Ctrl-d if needed (when something is
** typed and then the ctrlD is typed before the return button, it doesn't exit;
** otherwise it'll exit, either the command or the shell).
** gnl return:
** 1 = line read,
** 0 = EOF
** -1 = error
*/
void	print_tok(void *content)
{
	printf("unexpanded : |%s|\t\tfd0: %d\tfd1: %d\n",
		((t_command*)content)->unexpanded,
		((t_command*)content)->fd[0],
		((t_command*)content)->fd[1]);
}

/*
** Saving the $PATH var before any function is called, so if it's unset or
** exported as something else, we will have a copy of it to manage the error
** message for when the user types a command but $PATH is unset or not with it's
** correct paths.
*/
int	execute_cmd(t_list **env, t_command *cmd, t_list **export, char *s_path)
{
	int		ret;

	ret = is_builtin(cmd);
	if (ret)
		ret = execute_builtin(env, cmd, ret, export);
	else
		ret = execute_extern(env, cmd, s_path);
	close_fd(cmd->fd);
	return (ret);
}

/*
** print_cmd(cmd);//TEST - TO DELETE LATER
*/

int	executer(t_list **env, t_list *cmd, t_list **export, char *saved_path)
{
	int		ret;
	int		ret2;

	while (cmd)
	{
		ret = expander(env, (t_command *)((cmd)->content));
		if (ret < 0)
		{
			if (ret == RT_FAIL)
				return (RT_FAIL);
			return (RT_SUCCESS);
		}
		ret2 = execute_cmd(env,
				(t_command *)((cmd)->content), export, saved_path);
		if (ret2 != RT_SUCCESS)
		{
			if (ret2 == RT_NOEXIT)
				return (RT_SUCCESS);
			return (RT_EXIT);
		}
		cmd = cmd->next;
	}
	return (RT_SUCCESS);
}

/*
** Savinf $PATH env to be used later on in case the user does an "unset PATH"
** if the $PATH does not exist (because minishell was run as env -i ./minishell)
** then the saved_path will just be NULL, and the functions that need it will
** have to handle it as NULL;
** if $PATH does not exist at all (besides the env -i case), even the
** command "make" or "ld" won't be found, so in that case the user would not
** even be able to compile the program using the Makefile.
**
** Here I'm outputing a message showing the current $PATH just so the user
** knows that the $PATH is not set as t should be.
*/
char	*save_path_env(t_list **env)
{
	char	*saved_path;

	saved_path = find_env_value(env, "PATH");
	if (!(saved_path))
		return (NULL);
	if (ft_strstr(saved_path, "/bin") == NULL
		|| ft_strstr(saved_path, "/sbin") == NULL)
	{
		printf("\nCurrent PATH environment variable:\nPATH=%s\n\n", saved_path);
		return (ft_strdup(saved_path));
	}
	return (ft_strdup(saved_path));
}

int	main_loop(t_list **env, t_list **export)
{
	t_list	*cmd;
	char	*line;
	char	*saved_path;
	int		ret_gnl;

	signal(SIGQUIT, ctrl_back_slash_handler);
	saved_path = save_path_env(env);
	while (1)
	{
		ret_gnl = gnl_ctrld(0, &line);
		if (ret_gnl < 1)
			break ;
		if (verify_line(line))
			continue ;
		cmd = tokenizer(line);
		if (!(cmd))
			return (RT_FAIL);
		free(line);
		if (executer(env, cmd, export, saved_path) != RT_SUCCESS)
			return (clear_lists_exit(&cmd, env, saved_path));
		ft_lstclear(&cmd, &clear_commandlist);
	}
	return (return_to_main(env, line, ret_gnl, saved_path));
}

/*
** ft_lstiter(cmd, &print_tok);//TO DEL LATER
*/
