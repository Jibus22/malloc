#include "minishell.h"

int	token_pipe(char *line, int i, int space)
{
	char	*skiped;

	if (line[i + 1] == '|')
	{
		skiped = skip_char(&line[i + 2], ' ');
		if (!*skiped)
			return (UTOKEN_ML);
		if (*skiped == '|')
			return (UTOKEN_P);
		return (UTOKEN_OR);
	}
	if (space == 1)
		return (UTOKEN_P);
	skiped = skip_char(&line[i + 1], ' ');
	if (!*skiped)
		return (UTOKEN_ML);
	return (0);
}

int	token_sc(char *line, int i, int space)
{
	char	*skiped;

	if (line[i + 1] == ';')
		return (UTOKEN_DSC);
	if (space == 1)
		return (UTOKEN_SC);
	skiped = skip_char(&line[i + 1], ' ');
	if (*skiped == ';')
		return (UTOKEN_SC);
	return (0);
}
