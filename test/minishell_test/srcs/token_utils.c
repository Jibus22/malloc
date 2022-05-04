#include "minishell.h"

void	delete_remaining_char(char *str, char c)
{
	int		end_index;

	end_index = ft_strlen(str) - 1;
	if (str[end_index] == c)
		str[end_index] = 0;
}

t_command	*init_command(char *pipeline)
{
	int			*fd;
	t_command	*command;	

	fd = (int *)malloc(sizeof(int) * 4);
	if (!fd)
		return (NULL);
	fd[0] = 0;
	fd[1] = 1;
	fd[2] = 2;
	fd[3] = 0;
	command = (t_command *)malloc(sizeof(t_command) * 1);
	if (!command)
		return (0);
	command->fd = fd;
	command->unexpanded = pipeline;
	command->file = NULL;
	command->command = NULL;
	return (command);
}

char	*skip_char(char *str, char c)
{
	while (*str)
		if (*str++ != c)
			return (str - 1);
	return (str);
}

char	*end_of_object(char *str)
{
	while (*str)
	{
		if (*str == ' ' || *str == '>' || *str == '<')
			return (str);
		str++;
	}
	return (str);
}
