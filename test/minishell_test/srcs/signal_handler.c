#include "minishell.h"

/*
** When ctrl\ is pressed on an empty line it does nothing.
** \b = backspace
** So here after a ctrl\ is pressed, the cursos moves backwards 2 times, to
** erase ^\ the characters, then it prints 2 spaces to fillout the spaces these
** ^\ characters where taking, and last, it goes back again 2 times to put the
** cursor in place.
*/
void	ctrl_back_slash_handler(int signal)
{
	if (signal == SIGQUIT)
		ft_putstr_fd("\b\b  \b\b", 2);
}

void	ctrl_back_slash_handler_quit(int signal)
{
	if (signal == SIGQUIT)
	{
		g_exit_status = 131;
		ft_putstr_fd("Quit (core dumped)\n", 2);
	}
}

void	ctrl_c_handler(int sign)
{
	if (sign == SIGINT)
	{
		g_exit_status = 130;
		g_line_eraser = 1;
		ft_putstr_fd("\b\b\n\033[1;32mminishell$\033[0m ", 1);
	}
}
