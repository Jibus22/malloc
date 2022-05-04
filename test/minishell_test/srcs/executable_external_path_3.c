/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_external_path_3.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 19:16:50 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/16 14:15:40 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_found_more_times(t_command *cmd, char **split_path)
{
	int		i;

	i = -1;
	ft_putstr_fd("Command '", cmd->fd[2]);
	ft_putstr_fd(cmd->command[0], cmd->fd[2]);
	ft_putstr_fd("' is available in the following places\n", cmd->fd[2]);
	while (split_path[++i])
	{
		if (test_cmd(split_path[i], cmd->command[0]) == 0)
		{
			if (ft_strcmp(split_path[i], "/bin") == 0)
			{
				ft_putstr_fd(" * ", cmd->fd[2]);
				ft_putstr_fd(split_path[i], cmd->fd[2]);
				ft_putchar_fd('/', cmd->fd[2]);
				ft_putstr_fd(cmd->command[0], cmd->fd[2]);
				ft_putchar_fd('\n', cmd->fd[2]);
			}
		}
	}
}

/*
** The reading of $PATH is done from the index last found to right.
** To make it work starting from the next index after the last one found, we
** would have to user a global variable. I don't think we have to go that deep
** since the correction sheet asks only to check if we are checking the $PATH
** from left to right (not from where it stopped on the last command, to right).
*/

static int	split_the_path(char ***split_path, char *saved_path)
{
	*split_path = ft_split_jb(saved_path, ':');
	if (!(*split_path))
	{
		ft_freetab(*split_path);
		return (0);
	}
	return (1);
}

char	*test_path_left_right(t_command *cmd, char *saved_path)
{
	char	**split_path;
	int		ret_test;
	int		i;
	int		j;

	i = -1;
	j = 0;
	ret_test = 1;
	if (ft_strchr(saved_path, ':'))
	{
		if (!split_the_path(&split_path, saved_path))
			return (NULL);
	}
	else
		split_path = ft_split_jb(saved_path, '\0');
	while (split_path[++i])
	{
		ret_test = test_cmd(split_path[i], cmd->command[0]);
		if (ret_test == 0)
			j++;
	}
	if (!(test_path_left_right_2(cmd, split_path, ret_test, j)))
		return (NULL);
	return ("");
}
