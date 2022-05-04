#include "minishell.h"

int	return_to_main(t_list **env, char *line, int ret_gnl, char *saved_path)
{
	if (saved_path)
		free(saved_path);
	free(line);
	ft_lstclear(env, &clear_envlist);
	if (ret_gnl == -1)
		return (RT_FAIL);
	return (RT_EXIT);
}

static void	print_token_error(char *str)
{
	ft_putstr_fd("bash: syntax error near unexpected token `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("'\n", 2);
}

int	token_error_printer_hub(int err)
{
	if (!err)
		return (1);
	if (err == UTOKEN_P)
		print_token_error(E_UTOKEN_P);
	else if (err == UTOKEN_SC)
		print_token_error(E_UTOKEN_SC);
	else if (err == UTOKEN_ML)
		print_token_error(E_UTOKEN_ML);
	else if (err == UTOKEN_OR)
		print_token_error(E_UTOKEN_OR);
	else if (err == UTOKEN_RM)
		print_token_error(E_UTOKEN_RM);
	else if (err == UTOKEN_RL)
		print_token_error(E_UTOKEN_RL);
	else if (err == UTOKEN_RMM)
		print_token_error(E_UTOKEN_RMM);
	else if (err == UTOKEN_NL)
		print_token_error(E_UTOKEN_NL);
	else if (err == UTOKEN_DSC)
		print_token_error(E_UTOKEN_DSC);
	else if (err == UTOKEN_HD)
		print_token_error(E_UTOKEN_HD);
	return (0);
}

int	verify_line(char *line)
{
	char	*skiped;
	int		err;

	skiped = skip_char(line, ' ');
	if (!*skiped)
	{
		free(line);
		return (1);
	}
	err = 0;
	check_unexpected_token(line, &err);
	if (!(token_error_printer_hub(err)))
	{
		g_exit_status = 2;
		free(line);
		return (1);
	}
	return (0);
}
