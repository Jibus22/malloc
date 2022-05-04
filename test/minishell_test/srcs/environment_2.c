#include "minishell.h"

/*
 * Free each string from the t_env structure then free the structure.
** The parameters are made so that the function is usable with ft_lstdel.
**
** @args:
**		void *env: a pointer to the t_env struct to delete
**		size_t size: necessary for pointer to function.
*/

static void	free_env(void *env, size_t size)
{
	(void)size;
	free(((t_env *)env)->key);
	free(((t_env *)env)->value);
	free(env);
}

/*
** * Will find if a link from the env linked list contain a specific key,
** delete this link and rebuild the list.
**
** @args:
**		t_list **env: a pointer to the first link of the list
**		char *key: a string that will be compared against t_env->key
*/

void	delete_env_variable(t_list **env, char *key)
{
	t_list	*tmp_env;
	t_list	*tmp;

	if (!*env)
		return ;
	tmp_env = *env;
	while (tmp_env->next)
	{
		if (!ft_strcmp(key, ((t_env *)((tmp_env->next)->content))->key))
		{
			tmp = tmp_env->next->next;
			free_env(tmp_env->next->content, sizeof(env));
			free(tmp_env->next);
			tmp_env->next = tmp;
			break ;
		}
		tmp_env = tmp_env->next;
	}
	if (!ft_strcmp(key, ((t_env *)((*env)->content))->key))
	{
		tmp = (*env)->next;
		free_env((*env)->content, sizeof(env));
		free(*env);
		*env = tmp;
	}
}
