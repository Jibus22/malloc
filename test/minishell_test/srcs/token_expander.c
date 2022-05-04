#include "minishell.h"

static void	init_fr(char **line_ptr, t_command *cmd, t_list **arg, int *ret)
{
	*line_ptr = cmd->unexpanded;
	*line_ptr = skip_char(*line_ptr, ' ');
	*arg = NULL;
	*ret = 1;
}

/*
** -----------------------------------------------------------------------------
**
** %    echo     BLA    >   FILE1   POP  >   FILE2      FUF    -n
** % cat FILE2
** --->BLA POP FUF -n
**
** %  echo   "L  BLA   L" >FILE1 POP>FILE2       "L    FUF    L"       -n
** % cat FILE2
** --->L  BLA   L POP L    FUF    L -n
**
** % cat    -b  < FILE   -n       <=>       % cat -b -n < FILE
**
** % >FILE
** ---> just open the FILE (create if it doesn't exists & erase content if it
** exists)
**
** -----------------------------------------------------------------------------
**
** expander() is the last fonction which is called to finish the tokenize
** process. In fact, this is where the content of links from linked list ends up
** being created.
** This function handles file descriptors from redirections, then shaping of
** the array of command + arguments.
**
** Behavior :
** func must skip spaces. The first string met should be the name of the exe, so
** command[0].
** However, the name of the exe can be placed after the redirection, so that we
** have to have the following logic :
** - If a redirection symbol is met, the following string will be its filename.
**   Else, it will be joined to the command_string, which will then have to be
**   splited with spaces to make an array understandable to execution part.
**
** Now, after skiping spaces, we have to check the first character.
** - If this is double quotes, go into double_quote() ('$' expanded).
** - Or else if it is a simple quote, go into simple_quote().
** - Or into no_quotes() if no quotes ('\' ignored).
** A - Thoses functions treats the string differently,
** & returns a malloc() string.
**
** - Go into redirections() if it's a redirection symbol.
** B - This function opens file descriptor consequently & address it
** to fd_command
**
** A() & B() funcs receives the address of pointer to string so that they can
** make it points to the end of word-object from string after processing.
**
** By this way line_ptr keeps track of address after the end of a word object.
** It permits to continue the loop after a word object has been processed.
**
** After word_object-processing functions layer, we fetched in word_object a
** malloc() string which contains a word-object.
** We have to join this new word-object with the command_string.
**
** When the command_line has been fully processed through the line_ptr copy, we
** get outside of the loop and then can split the command_string with 'sp' to
** create an array which will be pointed by a new link of our linked list.
**
**
** t_list **cmd :		Pointer to t_list from the tokenize_line func which is
** 						initially NULL and which is here to point to new links.
** 						As we have the address of this pointer, we can
** 						make it point to a newly created link.
**
** t_list **env :		pointer to t_list, which contains environment. Used to
** 						expand environment variables symbolized with '$'
**
** char *command_line :	String already processed twice to creates semi-colon ';'
**						& pipes '|' object. At this stage, this string can
**						contains redirections, otherwise it's clean and only
**						contains commands (which can still have to be expanded).
**
** int *fd_command :	Array of int which has been malloc() & initialized just
**						before.
**						Either they have already been changed to piped fd,
**						either not, & they could be changed here by redirection.
**						The address pointed by this variable will be fetched
**						by the pointer to int created in the t_command struture
**						which will be malloc in the function which creates links
*/

static void	add_arg_to_list(t_list **arg, char *wobj)
{
	t_list		*new;

	if (!wobj)
		return ;
	new = ft_lstnew(wobj);
	ft_lstadd_front(arg, new);
}

static char	**convert_str_linkedlist_to_str_array(t_list *arg)
{
	int			size_list;
	char		**str_arr;

	size_list = ft_lstsize(arg);
	str_arr = (char **)malloc(sizeof(char *) * (size_list + 1));
	str_arr[size_list] = NULL;
	while (arg)
	{
		str_arr[--size_list] = arg->content;
		arg = arg->next;
	}
	return (str_arr);
}

static int	add_arglist_to_cmd(t_command *i_command, t_list **arg)
{
	char		**str_arr;

	str_arr = convert_str_linkedlist_to_str_array(*arg);
	ft_lstclear(arg, &clear_arglist);
	i_command->command = str_arr;
	return (1);
}

int	expander(t_list **env, t_command *i_command)
{
	int			ret;
	char		*line_ptr;
	char		*word_object;
	t_list		*arg;

	init_fr(&line_ptr, i_command, &arg, &ret);
	while (*line_ptr)
	{
		if (*line_ptr && (*line_ptr == '>' || *line_ptr == '<'))
		{
			ret = redirections(env, &line_ptr, i_command);
			if (ret < 0)
				return (return_expander(&arg, ret, i_command));
		}
		else
		{
			word_object = expand_token(env, &line_ptr);
			add_arg_to_list(&arg, word_object);
		}
		line_ptr = skip_char(line_ptr, ' ');
	}
	add_arglist_to_cmd(i_command, &arg);
	return (1);
}
