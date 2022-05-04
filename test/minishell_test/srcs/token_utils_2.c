#include "minishell.h"

int	authorized_char(int c)
{
	return (ft_isalnum(c) || c == '_');
}

int	is_special_char(int c)
{
	return (c == '=' || c == '+' || c == ':' || c == ',' || c == '%'
		|| c == '.' || c == '/' || c == ']' || c == '^' || c == '~'
		|| c == '[' || c == '(' || c == ')' || c == '`' || c == '{'
		|| c == '}');
}

int	is_symbol(int c)
{
	return (c == '>' || c == '<' || c == ' ' || c == '\'' || c == '\"');
}

int	is_symbol_doll(int c)
{
	return (c == '>' || c == '<' || c == ' '
		|| c == '\'' || c == '\"' || c == '$');
}

int	is_symbol_v2(int c)
{
	return (c == '>' || c == '<' || c == ' ');
}
