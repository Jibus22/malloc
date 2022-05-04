#include "minishell.h"

static int	check_symbol_nq(char *str, int i)
{
	if (i == 0)
		if (is_symbol(*str))
			return (0);
	if (i == 1)
		if (is_symbol(*str) && *(str - i) != '\\')
			return (0);
	return (1);
}

static int	expand_env_nq(t_list **env, char **final_str,
							char **str, char **line_ptr)
{
	if (!(join_str_before(str, line_ptr, final_str)))
		return (0);
	if (!(expand_doll_quote(env, str, final_str, 'n')))
		return (0);
	*line_ptr = *str;
	return (1);
}

static int	format_special_nq(t_list **env, char **final_str,
								char **str, char **line_ptr)
{
	if (**str == '\\')
	{
		if (!(escape_it(final_str, str, line_ptr)))
			return (-1);
		return (1);
	}
	else if (**str == '$' && *(*str + 1) != '\\')
	{
		if (!(expand_env_nq(env, final_str, str, line_ptr)))
			return (-1);
		return (1);
	}
	return (0);
}

/*
** no_quotes() process the given line to make it compatible with args of
** execve()
** The termination character for this formated word-object is the end of string
** or one of the characters met in is_symbol().
** It has to expand the dollar symbol if it is met.
** It modifies the address where the given pointer line_ptr points to, to  the
** end of the processed string (eol or is_symbol() char).
** It returns a malloc() address to a null-terminated string.
*/

static char	*end_no_quotes(char **str, char **line_ptr, char **final_str)
{
	if (!(join_str_before(str, line_ptr, final_str)))
		return (NULL);
	if (!**final_str)
		return (NULL);
	return (*final_str);
}

char	*no_quotes(t_list **env, char **line_ptr)
{
	char		*str;
	char		*final_str;
	int			i;
	int			ret;

	str = *line_ptr;
	final_str = ft_strnew(0);
	if (!final_str)
		return (NULL);
	i = 0;
	while (*str)
	{
		if (!check_symbol_nq(str, i))
			break ;
		i = 1;
		ret = format_special_nq(env, &final_str, &str, line_ptr);
		if (ret == -1)
			return (NULL);
		else if (ret == 1)
			continue ;
		else
			str++;
	}
	return (end_no_quotes(&str, line_ptr, &final_str));
}
