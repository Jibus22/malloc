#include "minishell.h"

/*
** ghosting make the characters into exception_set string be the 'ghoster'
** (or invisibiliser) of the next split character(s).
** Once a character from the set is met, all the c spliters won't be counted
** until this particular character will be met a second time.
**
** If a spliter is met, function stops & returns the adress in the string.
** Else if no splitter is met, it returns NULL.
** It returns an error if a character from the set is met only 1 time before
** the end of the string.
*/

int	handle_this_ghost(char *str, int i, char *ghoster, char *ghost)
{
	if (*ghost == 0)
	{
		if (i > 0 && str[i - 1] == '\\')
			return (0);
		*ghost = *ghoster;
	}
	else if (*ghoster == *ghost)
	{
		if (i > 0 && str[i - 1] == '\\' && str[i] == '\"')
			return (0);
		*ghost = 0;
	}
	return (1);
}

char	*ghosting(char *str, char c, char *exception_set, int *error)
{
	char	*ghoster;
	char	ghost;
	int		i;

	i = -1;
	ghost = 0;
	while (str && str[++i])
	{
		ghoster = ft_strchr(exception_set, str[i]);
		if (ghoster)
			if (!(handle_this_ghost(str, i, ghoster, &ghost)))
				continue ;
		if (str[i] == c && !ghost)
			if ((i > 0 && str[i - 1] != '\\') || i == 0)
				return (&(str[i]));
	}
	if (ghost && error)
		*error = -1;
	return (NULL);
}

char	**split_count_n_array_make(char *str, char c, char *exception_set)
{
	char	**split_array;
	int		count;
	int		i;

	count = 1;
	i = 1;
	while (1)
	{
		str = ghosting(str, c, exception_set, &i);
		if (!str)
			break ;
		count++;
		str++;
	}
	if (i == -1)
		return (NULL);
	split_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!(split_array))
		return (NULL);
	return (split_array);
}

/* Current behavior of split_with_exception: if this line is received >;;
** split_with_exception will see >; >; >'\0' >null
** So the two first strings created  will contain ';' & the third '\0' & the
** last will be null.
**
** Behavior with no semi-colon :	>hello
** 									-splited =>hello'\0' >null
**
** Behavior with one semi-colon :	>hello;
** 									splited =>hello;'\0' >'\0' >null
**
** If it receives nothing (type 'enter' means a "\0" string from gnl) >
** It will create 1 string fullfiled with a '\0'
** In minishell behavior, the less we can receive is a "\0" string.
**
**
** A string like >"hello 'you'; :) how are you" bla ; 'my' name "is;" toto
** will give this : 1 - >|"hello 'you'; :) how are you" bla ;|<
** 					2 - >| 'my' name "is;" toto|<
** 					3 - >|null
** The array is always null-terminated.
**
** This function takes a set of characters which will indicates to spliter
** to ghost the spliting character.
** The behavior is the following :
**
** 	-	We split the line with the spliter character 'c', but this character is
** 		kept at the end of the line.
** 	-	If a character from the exception_set is met, we won't split the line
** 		at the next meeting of spliter character 'c' until we meet a second time
** 		the character from exception_set that we already met a first time.
** 	-	If the corresponding exception_set character is never met, it returns
** 		an error. (actually, NULL)
**
** Returns a null terminated array of null terminated strings.
*/
char	**split_with_exception(char *str, char c, char *exception_set)
{
	char	**split_array;
	char	*c_position;
	int		i;

	split_array = split_count_n_array_make(str, c, exception_set);
	c_position = str;
	i = -1;
	while (1)
	{
		c_position = ghosting(c_position, c, exception_set, NULL);
		if (!c_position)
			break ;
		split_array[++i] = ft_substr(str, 0, (c_position - str));
		str = ++c_position;
	}
	if (!c_position && str)
	{
		split_array[++i] = ft_substr(str, 0, ft_strlen(str));
		if (!*(skip_char(split_array[i], ' ')))
			ft_strdel(&(split_array[i]));
	}
	split_array[++i] = NULL;
	return (split_array);
}
