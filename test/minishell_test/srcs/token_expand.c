#include "minishell.h"

char	*expand_doll_digit(char digit)
{
	char		*expanded;

	if (digit != '0')
		expanded = ft_strnew(0);
	else
		expanded = ft_strdup("minishell");
	return (expanded);
}

char	*expand_exit_status(void)
{
	char		*expanded;

	expanded = ft_itoa(g_exit_status);
	if (!expanded)
		return (NULL);
	return (expanded);
}

char	*check_special_value(char **line_ptr, char quote)
{
	char		*value;

	if (ft_isdigit(**line_ptr))
		value = expand_doll_digit(**line_ptr);
	else if (**line_ptr == '?')
		value = expand_exit_status();
	else if (!**line_ptr || is_symbol_doll(**line_ptr))
	{
		if (quote == 'n' && (**line_ptr == '\"' || **line_ptr == '\''))
			return (value = ft_strnew(0));
		else
			return (value = ft_strdup("$"));
	}
	else if (is_special_char(**line_ptr))
		value = ft_substr(*line_ptr - 1, 0, 2);
	else
		return (NULL);
	*line_ptr += 1;
	return (value);
}

char	*doll_expand(t_list **env, char **line_ptr, char quote)
{
	char		*doll;
	char		*str_env;
	char		*value;

	*line_ptr += 1;
	doll = *line_ptr;
	value = NULL;
	value = check_special_value(line_ptr, quote);
	if (value)
		return (value);
	while (**line_ptr && !is_symbol_doll(**line_ptr)
		&& authorized_char(**line_ptr))
		*line_ptr += 1;
	doll = ft_substr(doll, 0, (*line_ptr - doll));
	if (!doll)
		return (NULL);
	str_env = find_env_value(env, doll);
	if (!str_env)
		value = ft_strnew(0);
	if (!value)
		value = ft_strdup(str_env);
	free(doll);
	return (value);
}

int	expand_doll_quote(t_list **env, char **str,
		char **final_str, char quote)
{
	char		*expanded;

	expanded = doll_expand(env, str, quote);
	if (!expanded)
		return (0);
	if (!(join_newstr(final_str, expanded)))
		return (0);
	free(expanded);
	return (1);
}
