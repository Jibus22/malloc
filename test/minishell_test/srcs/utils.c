#include "minishell.h"

char	*last_arg(t_command *cmd)
{
	int		i;

	i = 0;
	while (cmd->command[i])
		i++;
	if (i > 0)
		i -= 1;
	return (cmd->command[i]);
}

int	update_underscore(t_list **env, char *path_cmd)
{
	char	*keyvalue;

	keyvalue = ft_strjoin("_=", path_cmd);
	if (!(keyvalue))
		return (1);
	add_env_variable(env, keyvalue);
	free(keyvalue);
	return (1);
}

int	ft_count_tab(char **tab)
{
	int	i;

	i = 0;
	if (tab == NULL)
		return (0);
	while (tab[i] != NULL)
		i++;
	return (i);
}

/*
** while the next index array of the env command exists, compare the current
** index with the next one (meaning envir[i] is a char bigger than envir[i + 1].
** ex B, A), then, swap the lines.
*/
char	**alpha_order_array(char **export_tab)
{
	char	*export_line;
	int		i;

	i = 0;
	export_line = NULL;
	while (export_tab[i + 1])
	{
		if (ft_strcmp(export_tab[i], export_tab[i + 1]) > 0)
		{
			export_line = export_tab[i];
			export_tab[i] = export_tab[i + 1];
			export_tab[i + 1] = export_line;
			i = 0;
		}
		else
			i++;
	}
	return (export_tab);
}
