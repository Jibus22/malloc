/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_error_manager.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 19:34:20 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/16 00:44:53 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** @args:
**		1. char *bash: A string to define if "bash: " will be outputed.
**		2. t_command *cmd: A pointer to the t_command struct containing the
**			parsed command that came from the tokenizer.
**		3. char *arg: A string to define if the first argument of a command
**			will be outputed, as "arg: ".
**		4. char *err_msg: A string to define if either a chosen error message
**			or a strerror(errno) error message will be outputed.
**
** Output:
** <bash: if needed> <cmd->command[0]>: <arg/cmd->command[1] if needed>:
** <err_msg if strerror(errno) does not output the correct error message\n> OR
** <strerror(errno)\n> if it outputs the expected message (we have to
** check it manually before or on the go).
*/

void	error_msg(char *bash, t_command *cmd, char *arg, char *err_msg)
{
	if (bash)
		ft_putstr_fd("bash: ", cmd->fd[2]);
	ft_putstr_fd(cmd->command[0], cmd->fd[2]);
	ft_putstr_fd(": ", cmd->fd[2]);
	if (arg)
	{
		ft_putstr_fd(arg, cmd->fd[2]);
		ft_putstr_fd(": ", cmd->fd[2]);
	}
	if (err_msg)
	{
		ft_putstr_fd(err_msg, cmd->fd[2]);
		ft_putchar_fd('\n', cmd->fd[2]);
	}
}

int	error_msg_2(char *bash, t_command *cmd, char *arg, char *err_msg)
{
	if (bash)
		ft_putstr_fd("bash: ", cmd->fd[2]);
	if (arg)
	{
		ft_putstr_fd(arg, cmd->fd[2]);
		ft_putstr_fd(": ", cmd->fd[2]);
	}
	if (err_msg)
	{
		ft_putstr_fd(err_msg, cmd->fd[2]);
		ft_putchar_fd('\n', cmd->fd[2]);
	}
	return (RT_SUCCESS);
}
