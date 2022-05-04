/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_external_path_2.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 19:16:50 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/16 00:33:23 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cmd_found_once(t_command *cmd, char **split_path, int j)
{
	int		i;

	i = -1;
	while (split_path[++i])
	{
		if (test_cmd(split_path[i], cmd->command[0]) == 0)
		{
			j++;
			ft_putstr_fd("Command '", cmd->fd[2]);
			ft_putstr_fd(cmd->command[0], cmd->fd[2]);
			ft_putstr_fd("' is available in '", cmd->fd[2]);
			ft_putstr_fd(split_path[i], cmd->fd[2]);
			ft_putchar_fd('/', cmd->fd[2]);
			ft_putstr_fd(cmd->command[0], cmd->fd[2]);
			ft_putstr_fd("'\nThe command ", cmd->fd[2]);
			ft_putstr_fd("could not be located because '", cmd->fd[2]);
			ft_putstr_fd(split_path[i], cmd->fd[2]);
			ft_putstr_fd("' is not included in the ", cmd->fd[2]);
			ft_putstr_fd("PATH environment variable.\n", cmd->fd[2]);
		}
	}
}

static void	cmd_found_more_times_1(t_command *cmd, char **split_path)
{
	int		i;

	i = -1;
	while (split_path[++i])
	{
		if (test_cmd(split_path[i], cmd->command[0]) == 0)
		{
			if (ft_strcmp(split_path[i], "/bin") != 0)
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

static void	cmd_found_more_times_2(t_command *cmd, char **split_path)
{
	int		i;
	int		j;

	i = -1;
	j = 0;
	ft_putstr_fd("The command could not be located because '", cmd->fd[2]);
	while (split_path[++i])
	{
		if (test_cmd(split_path[i], cmd->command[0]) == 0)
		{
			ft_putstr_fd(split_path[i], cmd->fd[2]);
			j = i;
			while (split_path[j])
			{
				if (split_path[j + 1])
					if (test_cmd(split_path[j + 1], cmd->command[0]) == 0)
						ft_putchar_fd(':', cmd->fd[2]);
				j++;
			}
		}
	}
	ft_putstr_fd("' is not included in the ", cmd->fd[2]);
	ft_putstr_fd("PATH environment variable.\n", cmd->fd[2]);
}

static void	cmd_found_more_times_3(t_command *cmd, char **split_path)
{
	int		i;

	i = -1;
	while (split_path[++i])
	{
		if (test_cmd(split_path[i], cmd->command[0]) == 0)
		{
			if (ft_strcmp(split_path[i], "/sbin") == 0)
			{
				ft_putstr_fd("This is most likely caused by the ", cmd->fd[2]);
				ft_putstr_fd("lack of administrative privileges ", cmd->fd[2]);
				ft_putstr_fd("associated with your user account.", cmd->fd[2]);
				ft_putchar_fd('\n', cmd->fd[2]);
				break ;
			}
		}
	}
}

void	*test_path_left_right_2(t_command *cmd, char **split, int rt, int j)
{
	if (rt == -1 && j == 0)
	{
		error_msg("bash", cmd, NULL, strerror(2));
		ft_freetab(split);
		return (NULL);
	}
	if (j == 1)
		cmd_found_once(cmd, split, j);
	else if (j > 1)
	{
		cmd_found_more_times(cmd, split);
		cmd_found_more_times_1(cmd, split);
		cmd_found_more_times_2(cmd, split);
		cmd_found_more_times_3(cmd, split);
	}
	ft_freetab(split);
	return ("");
}
