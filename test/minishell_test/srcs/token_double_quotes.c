#include "minishell.h"

static int	check_symbol_dq(char *str, int i)
{
	if (i == 0)
		if (*str == '\"')
			return (0);
	if (i == 1)
		if (*str == '\"' && *(str - i) != '\\')
			return (0);
	return (1);
}

static int	expand_env_dq(t_list **env, char **final_str,
							char **str, char **line_ptr)
{
	if (!(join_str_before(str, line_ptr, final_str)))
		return (0);
	if (!(expand_doll_quote(env, str, final_str, 'y')))
		return (0);
	*line_ptr = *str;
	return (1);
}

static int	format_special_dq(t_list **env, char **final_str,
								char **str, char **line_ptr)
{
	if (**str == '\\' && (*(*str + 1) == '\"'
			|| *(*str + 1) == '\\' || *(*str + 1) == '$'))
	{
		if (!(escape_it(final_str, str, line_ptr)))
			return (-1);
		return (1);
	}
	else if (**str == '$' && *(*str + 1) != '\\')
	{
		if (!(expand_env_dq(env, final_str, str, line_ptr)))
			return (-1);
		return (1);
	}
	return (0);
}

/*
** double_quotes() process the given line to make it compatible with args of
** execve()
** The termination character for this formated word-object is the end of string
** or the double-quote.
** It has to expand the dollar symbol if it is met.
** It modifies the address where the given pointer line_ptr points to the end of
** the processed string, after the quote.
** It returns a malloc() address to a null-terminated string.
*/

static int	init_dq(char **final_str, char **str, char **line_ptr, int *i)
{
	*final_str = ft_strnew(0);
	if (!(*final_str))
		return (0);
	*str = *line_ptr + 1;
	*i = 0;
	return (1);
}

char	*double_quotes(t_list **env, char **line_ptr)
{
	char		*str;
	char		*final_str;
	int			i;
	int			ret;

	if (!(init_dq(&final_str, &str, line_ptr, &i)))
		return (NULL);
	while (*str)
	{
		if (!check_symbol_dq(str, i))
			break ;
		i = 1;
		ret = format_special_dq(env, &final_str, &str, line_ptr);
		if (ret == -1)
			return (NULL);
		else if (ret == 1)
			continue ;
		else
			str++;
	}
	if (*str)
		str++;
	if (!(join_str_before(&str, line_ptr, &final_str)))
		return (NULL);
	return (final_str);
}
