#include "minishell.h"

int	verify_line_2(char *line)
{
	char	*skiped;
	int		err;

	skiped = skip_char(line, ' ');
	if (!*skiped)
		return (1);
	err = 0;
	check_unexpected_token(line, &err);
	if (!(token_error_printer_hub(err)))
	{
		g_exit_status = 2;
		return (1);
	}
	return (0);
}

int	arg_command(t_list **env, t_list **export, int ac, char **av)
{
	t_list	*cmd;
	char	*saved_path;

	if (ac != 3)
	{
		ft_putstr_fd("bash: need two arguments\n", 2);
		return (RT_EXIT);
	}
	signal(SIGQUIT, ctrl_back_slash_handler);
	saved_path = save_path_env(env);
	if (verify_line(av[2]))
		return (RT_EXIT);
	cmd = tokenizer(av[2]);
	if (!(cmd))
		return (RT_FAIL);
	executer(env, cmd, export, saved_path);
	clear_lists_exit(&cmd, env, saved_path);
	return (g_exit_status);
}

/*
** ft_lstiter(cmd, &print_tok);//TO DEL LATER
**
*/
