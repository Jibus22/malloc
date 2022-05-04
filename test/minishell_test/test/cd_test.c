#include "minishell.h"

int		main(int ac, char *av[], char *ep[])
{
	t_command	cmd;
	char		*pwd;
    t_list		*list;
	char		*key_pwd;
	char		*key_oldpwd;

	if(ac != 2)
		printf("it must be one arg\n");
    list = create_env_list(ep);
	key_pwd = find_env_value(&list, "PWD");
	key_oldpwd = find_env_value(&list, "OLDPWD");
	printf("Values before :\n-- PWD\t\t%s\n-- OLDPWD\t%s\n", key_pwd, key_oldpwd);
	cmd.command = (char**)malloc(sizeof(char*) * 2);
	cmd.command[1] = av[1]; 
	cmd.command[1] = malloc(sizeof(char) * 18);
	ft_strlcpy(cmd.command[1], "~////", 8);
	pwd = getcwd(NULL, 0);
	printf("\ncurrent dir before: %s\n\n", pwd);
	free(pwd);
	cd_builtin(&list, &cmd);
	pwd = getcwd(NULL, 0);
	printf("\ncurrent dir after: %s\n\n", pwd);
	key_pwd = find_env_value(&list, "PWD");
	key_oldpwd = find_env_value(&list, "OLDPWD");
	printf("Values after :\n-- PWD\t\t%s\n-- OLDPWD\t%s\n", key_pwd, key_oldpwd);
	free(cmd.command);
	free(pwd);
    ft_lstdel(&list, free_env);
	//system("leaks testit");
	return (0);
}
