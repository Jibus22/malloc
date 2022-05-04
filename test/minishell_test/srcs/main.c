#include "minishell.h"

/*
** Because when ctrlD or exit builtinn is typed, they close the shell and
** decrease this $SHLVL environmental variable. Even though this $SHLVL env
** does not close the shell/window when its exported to any number, I think
** that it exists just for the record to know how many levels of shell there
** are currenclty in the current tab/window.
*/
int	increase_shlvl(t_list **env)
{
	char	*keyvalue;
	char	*shlvl_str;
	int		shlvl_nb;

	shlvl_str = find_env_value(env, "SHLVL");
	if (!(shlvl_str))
	{
		add_env_variable(env, "SHLVL=1");
		return (0);
	}
	shlvl_nb = ft_atoi(shlvl_str);
	shlvl_nb++;
	shlvl_str = ft_itoa(shlvl_nb);
	if (!(shlvl_str))
		return (RT_FAIL);
	keyvalue = ft_strjoin("SHLVL=", shlvl_str);
	if (!(keyvalue))
	{
		ft_strdel(&shlvl_str);
		return (RT_FAIL);
	}
	ft_strdel(&shlvl_str);
	add_env_variable(env, keyvalue);
	ft_strdel(&keyvalue);
	return (0);
}

int	set_pwd(t_list **env)
{
	char	*cwd;
	char	*pwd;

	cwd = find_env_value(env, "PWD");
	if ((cwd))
		return (1);
	cwd = getcwd(NULL, 0);
	pwd = ft_strjoin("PWD=", cwd);
	if (!(pwd))
	{
		ft_strdel(&cwd);
		return (RT_FAIL);
	}
	ft_strdel(&cwd);
	add_env_variable(env, pwd);
	ft_strdel(&pwd);
	return (0);
}

char	**export_env(char **ep)
{
	char	**export_tab;
	int		i;
	int		j;

	i = 0;
	j = 0;
	export_tab = (char **)malloc(sizeof(char *) * ft_count_tab(ep) + 1);
	if (!(export_tab))
		return (NULL);
	while (ep[i])
	{
		if (ep[i][0] != '_')
			export_tab[j++] = ft_strdup(ep[i]);
		i++;
	}
	export_tab[j] = NULL;
	return (export_tab);
}

int	main(int ac, char **av, char **ep)
{
	t_list	*env;
	t_list	*export;
	char	**export_env_tab;

	if (ac > 1 && av)
		return (0);
	env = create_env_list(ep);
	if (increase_shlvl(&env) == RT_FAIL)
		return (RT_FAIL);
	if (set_pwd(&env) == RT_FAIL)
		return (RT_FAIL);
	export_env_tab = export_env(ep);
	export = create_env_list(export_env_tab);
	ft_freetab(export_env_tab);
	if (increase_shlvl(&export) == RT_FAIL)
		return (RT_FAIL);
	if (set_pwd(&export) == RT_FAIL)
		return (RT_FAIL);
	g_exit_status = 0;
	g_line_eraser = 0;
	if (main_loop(&env, &export) == RT_FAIL)
		return (RT_FAIL);
	ft_lstclear(&export, &clear_envlist);
	return (g_exit_status);
}

/*
** system("leaks minishell");
*/
