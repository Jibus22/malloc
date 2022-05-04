#include "minishell.h"

static int	create_env_struct(char *keyvalue, t_env *new)
{
	char	*needle;

	needle = ft_strchr(keyvalue, '=');
	if (!needle)
	{
		new->key = ft_strdup(keyvalue);
		new->value = ft_strdup("");
	}
	else
	{
		new->key = ft_substr(keyvalue, 0, ft_strlen(keyvalue)
				- ft_strlen(needle));
		new->value = ft_strdup(needle);
	}
	return (1);
}

/*
** Create and store environment in a linked list of t_env from envp variable.
** All the links of the list are malloc() and should be free() once the program
** exit. If the first arg was malloc(), it can be free() after.
**
** @args:
**		char **envp a string table containing entries with the format KEY=VALUE
** @return:
**		t_list *: a pointer to the head of a list.
*/

t_list	*create_env_list(char **envp)
{
	t_env	*new;
	t_list	*env;
	t_list	*tmp_env;

	env = NULL;
	while (*envp != NULL)
	{
		new = (t_env*)malloc(sizeof(t_env));
		if (!(new))
			return (NULL);
		create_env_struct(*envp, new);
		tmp_env = ft_lstnew(new);
		ft_lstadd_front(&env, tmp_env);
		envp++;
	}
	return (env);
}

/*
** Return a table similar to envp from a linked list of t_env.
** The table is malloc() and should be free() after use.
** The result should correspond to the return of the `env` builtin.
**
** @args:
**		t_list *env: a pointer to the head of a linked list.
** @return:
**		char **: a string table containing entries with the format KEY=VALUE
*/

char	**env_list_to_tab(t_list *env)
{
	int		count;
	char	**new_env;
	char	*tmp_str;

	if (!env)
		return (NULL);
	count = ft_lstsize(env);
	new_env = (char **)malloc(sizeof(char *) * (count + 1));
	if (!(new_env))
		return (NULL);
	new_env[count] = NULL;
	while (--count >= 0 && env != NULL)
	{
		tmp_str = ft_strdup(((t_env *)((env)->content))->key);
		new_env[count] = ft_strjoin(tmp_str,
				((t_env *)((env)->content))->value);
		free(tmp_str);
		env = env->next;
	}
	return (new_env);
}

/*
** Create a new t_env from a string (format: KEY=VALUE) and push it on top of
** the linked list.
** The string is duplicated and can be free() after use. If the key already
** exist its old value is replaced by the new one.
**
** @args:
**		t_list **env: a pointer to the first link of the list.
**		char *var: a string with the format KEY=VALUE
*/

void	add_env_variable(t_list **env, char *var)
{
	t_env	*new;
	t_list	*tmp_env;

	new = (t_env*)malloc(sizeof(t_env));
	if (!create_env_struct(var, new))
		return ;
	tmp_env = *env;
	while (tmp_env)
	{
		if (!ft_strcmp(new->key, ((t_env *)((tmp_env)->content))->key))
		{
			free(new->key);
			free(((t_env *)((tmp_env)->content))->value);
			((t_env *)((tmp_env)->content))->value = new->value;
			free(new);
			break ;
		}
		tmp_env = tmp_env->next;
	}
	if (tmp_env == NULL)
	{
		tmp_env = ft_lstnew(new);
		ft_lstadd_front(env, tmp_env);
	}
}

/*
** Will find a value if the key is stored inside a linked_list of t_env.
** The string is NOT duplicated and shouldn't be free() after use.
**
** @args:
**		t_list **head: a pointer to the first link of the list.
**		char *key: a string that will be compared against t_env->key
** @return:
**		char *: a string that correspond to the key that was sent.
*/

char	*find_env_value(t_list **env, char *key)
{
	t_list	*tmp_env;

	tmp_env = *env;
	while (tmp_env)
	{
		if (!ft_strcmp(key, ((t_env *)((tmp_env)->content))->key))
			return (&((t_env *)((tmp_env)->content))->value[1]);
		tmp_env = tmp_env->next;
	}
	return (NULL);
}
