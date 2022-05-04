#include "minishell.h"

int	join_str_before(char **str, char **line_ptr, char **final_str)
{
	char		*tmp;

	tmp = ft_substr(*line_ptr, 0, (*str - *line_ptr));
	if (!tmp)
		return (0);
	if (!(join_newstr(final_str, tmp)))
		return (0);
	free(tmp);
	*line_ptr = *str;
	return (1);
}

int	join_char_to_str(char **final_str, char c)
{
	char		*tmp;
	char		str[2];

	str[0] = c;
	str[1] = 0;
	tmp = ft_strjoin(*final_str, str);
	if (!tmp)
		return (0);
	free(*final_str);
	*final_str = tmp;
	return (1);
}

int	escape_it(char **final_str, char **str, char **line_ptr)
{
	if (!(join_str_before(str, line_ptr, final_str)))
		return (0);
	if (!(join_char_to_str(final_str, *(*str + 1))))
		return (0);
	if (*(*str + 1))
		*str += 2;
	else
		*str += 1;
	*line_ptr = *str;
	return (1);
}
