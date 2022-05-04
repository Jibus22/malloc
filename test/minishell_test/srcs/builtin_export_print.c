#include "minishell.h"

static void	print_export_2(char **alpha_order, int len, int i, int *fd)
{
	char	*key;

	key = ft_substr(alpha_order[i], 0, sizeof(char) * (len + 1));
	ft_putstr_fd("declare -x ", fd[1]);
	ft_putstr_fd(key, fd[1]);
	ft_putchar_fd('\"', fd[1]);
	ft_putstr_fd(&alpha_order[i][len + 1], fd[1]);
	ft_putstr_fd("\"\n", fd[1]);
	free(key);
}

void	print_export(char **export_tab, int *fd)
{
	char	**alpha_order;
	int		i;
	int		len;

	i = 0;
	len = 0;
	alpha_order = alpha_order_array(export_tab);
	if (!alpha_order)
		return ;
	while (alpha_order[i])
	{
		len = ft_strclen(&alpha_order[i][0], '=');
		if (ft_strchr(&alpha_order[i][0], '='))
			print_export_2(alpha_order, len, i, fd);
		else if (ft_strchr(&alpha_order[i][0], '=') == NULL)
		{
			ft_putstr_fd("declare -x ", fd[1]);
			ft_putstr_fd(alpha_order[i], fd[1]);
			ft_putchar_fd('\n', fd[1]);
		}
		i++;
	}
}
