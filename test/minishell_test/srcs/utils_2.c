#include "minishell.h"

static int	is_clearable(char c)
{
	return (c == ' ' || c == '\n' || c == '\t' || c == '\v'
		|| c == '\f' || c == '\r');
}

static int	is_skippable(char c)
{
	return (c == '-' || c == '+');
}

int	ternarie_return(int sign)
{
	if (sign > 0)
		return (-1);
	else
		return (0);
}

long long int	ft_long_long_atoi(const char *str)
{
	unsigned long long int	res;
	unsigned long long int	sign;
	int						i;

	sign = 1;
	res = 0;
	while (*str && is_clearable(*str))
		str++;
	if (*str == '-')
		sign = -1;
	if (is_skippable(*str))
		str++;
	i = 0;
	while (ft_isdigit(*str))
	{
		res = res * 10 + (*str - '0');
		str++;
		i++;
	}
	if (i > 19 || res >= 9223372036854775808ULL)
		return (ternarie_return(sign));
	return (res * sign);
}
