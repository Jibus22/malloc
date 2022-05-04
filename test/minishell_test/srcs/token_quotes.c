#include "minishell.h"

/*
** simple_quotes() process the given line to make it compatible with args of
** execve()
** The termination character for this formated word-object is the end of string
** or the simple-quote.
** It doesn"t have to expand dollar character.
** It modify the address where the given pointer line_ptr points to the end of
** the processed string, after the quote.
** It returns a malloc() address to a null-terminated string.
*/

char	*simple_quotes(char **line_ptr)
{
	char		*word_object;
	char		*tmp;

	tmp = *line_ptr;
	*line_ptr += 1;
	while (**line_ptr != '\'')
		*line_ptr += 1;
	*line_ptr += 1;
	word_object = ft_substr(tmp, 0, (*line_ptr - tmp));
	if (!word_object)
		return (NULL);
	return (word_object);
}

char	*quotes(t_list **env, char **line_ptr)
{
	char		*word_object;

	if (**line_ptr == '\'')
		word_object = simple_quotes(line_ptr);
	else
		word_object = double_quotes(env, line_ptr);
	if (!word_object)
		return (NULL);
	return (word_object);
}
