#include "minishell.h"

static int	is_token(int c)
{
	return (c == '|' || c == '<' || c == '>' || c == ';');
}

static int	token_hub(char *line, int i, int space)
{
	int		err;

	err = 0;
	if (line[i] == '|')
		err = token_pipe(line, i, space);
	else if (line[i] == '>')
		err = token_rm(line, i);
	else if (line[i] == '<')
		err = token_rl(line, i);
	else if (line[i] == ';')
		err = token_sc(line, i, space);
	return (err);
}

static int	handle_quotes(char *line, int i, char *quote)
{
	if (!*quote)
	{
		if (i > 0 && line[i - 1] == '\\')
			return (1);
		*quote = line[i];
	}
	else if (*quote == line[i])
	{
		if (i > 0 && line[i] == '\"' && line[i - 1] == '\\')
			return (1);
		*quote = 0;
	}
	return (1);
}

int	check_unexpected_token(char *line, int *err)
{
	char	quote;
	int		space;
	int		i;

	i = -1;
	quote = 0;
	space = 0;
	while (line[++i])
	{
		if (line[i] != ' ')
			space++;
		if (line[i] == '\'' || line[i] == '\"')
			handle_quotes(line, i, &quote);
		if (!quote && is_token(line[i]))
		{
			*err = token_hub(line, i, space);
			if (*err)
				return (*err);
			space = 0;
		}
	}
	if (quote)
		return ((*err = UTOKEN_ML));
	return (0);
}
