#include "minishell.h"

int	token_rmm(char *line, int i)
{
	char	*skiped;

	skiped = skip_char(&line[i + 1], ' ');
	if (*skiped == '>')
		return (UTOKEN_RM);
	else if (!*skiped)
		return (UTOKEN_NL);
	return (0);
}

int	token_rm(char *line, int i)
{
	char	*skiped;

	if (line[i + 1] == '>')
		return (token_rmm(line, i + 1));
	skiped = skip_char(&line[i + 1], ' ');
	if (*skiped == '>')
		return (UTOKEN_RM);
	else if (!*skiped)
		return (UTOKEN_NL);
	return (0);
}

int	token_rll(char *line, int i)
{
	char	*skiped;

	if (line[i + 1] == '<')
		return (UTOKEN_NL);
	skiped = skip_char(&line[i + 1], ' ');
	if (*skiped == '<')
		return (UTOKEN_RL);
	else if (!*skiped)
		return (UTOKEN_NL);
	return (UTOKEN_HD);
}

int	token_rl(char *line, int i)
{
	char	*skiped;

	if (line[i + 1] == '<')
		return (token_rll(line, i + 1));
	skiped = skip_char(&line[i + 1], ' ');
	if (*skiped == '<')
		return (UTOKEN_RL);
	else if (!*skiped)
		return (UTOKEN_NL);
	return (0);
}
