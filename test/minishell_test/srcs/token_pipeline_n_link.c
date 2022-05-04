#include "minishell.h"

/*
** parse_token_error() handle some error cases for pipe like these lines :
** 	1-	 |ls| rev			>| >ls| >rev
** 	2-	ls|  rev| | wc		>ls| >rev| > | >wc
** 	3-	ls |||rev			>ls| >| >| >rev
** 	4-	ls  ||				>ls| >|					-> multiline not handled.
** 	5-	ls| rev | 			>ls| >rev|				-> multiline not handled.
** 	These error cases give "bash: syntax error near unexpected token `|'" 
**
** 	6-	ls|rev||wc		>ls| >rev| >| >wc		-> 'or' not handled.
** 	In the case above, we have a logical 'or' which we don't handle, but we
** 	still must make the commands before 'or' work. In this case the function
** 	replaces the first character of all following splited lines by '\0' to make
** 	them invisible & by this way not usable.
*/

/*
** pipe_it()
** The array received as first parameter is the array of splited string with
** pipe symbol '|'
** It means that each string represents a set of commmand which can have their
** reading and/or writing file descriptor changed to a piped file descriptor.
**
** If there is string after the current one, it means the current one must
** write into a pipe where the next one will read.
** Else, the current one can write into the standard fd.
**
** If i > 0, in other words, if their was a command before the current one, it
** means a pipe had been opened before to write into, so the current one must
** read into the read end pipe created before, which had been saved in fd_tmp.
*/
static int	pipe_it(char **pipeline, int i,
		int *fd_command, int *fd_tmp)
{
	int			piped_fd[2];

	if (pipeline[i + 1])
	{
		if (pipe(piped_fd) == -1)
			fd_command[3] = errno;
		fd_command[1] = piped_fd[1];
	}
	if (i > 0)
		fd_command[0] = *fd_tmp;
	*fd_tmp = piped_fd[0];
	return (1);
}

/*
** find_pipe_n_redirections()
** Process the given line to split it, if a '|' is found.
** Behavior : if it receives this >ls | rev | wc -c
** Make this :	->ls |'\0'
** 				-> rev |'\0'
** 				-> wc -c'\0'
** 				->NULL
**
** If it receives this >ls | rev | wc -c|
** Make this :	->ls |'\0'
** 				-> rev |'\0'
** 				-> wc -c|'\0'
** 				->'\0'
** 				->NULL
**
** It ignores '|' which are between two same quotes.
** It handle distribution of piped file descriptors to commands & call
** the find_redirections function which handles distribution of opened-file
** file descriptor and $ expand.
*/

/*
** tokenize_line() starts the tokenize process.
** Priority spliter order : 1 - ;
** 							2 - |
** 							3 - >, >>, <
** Then $ expansion comes (if it's not inside simple quotes '')
**
** A -	Split the command line depending on ; char, if it's not inside quotes.
** B -	Process each lines to find pipes, then redirection, which will result to
** 		give them pipe() created fd, or open() created fd.
** C -	Called by B(), expand $
**
**
** -----------------------------------------------------------------------------
**
**
** Returns an adress to the head of a t_list{} linked list, with following
** content inside:
**
**
** 		- t_command structure which contains itself 1-> an array of strings
** 													2-> an array of int
**
**
** 1- char	**command	-	command[0] must contains the name of executable.
** 							This name could include the path, which should
** 							be parsed by the executor function.
** 						-	command[n] must contains args passed to the exe.
** 					 	-	command[last] = NULL;
** 2- int	*fd			-	will be malloc() as an array of 3 int.
** 						-	fd[0] - fd from which exe reads.
** 						-	fd[1] - fd to which exe write.
** 						-	fd[2] - fd to which exe write errors.
**
** -----------------------------------------------------------------------------
**
** To redirect fd to which called exe will read or write (to/from a file fd or
** to/from a pipe fd (=stream)) we will use the following way :
** - dup2(open_fd, standard_fd)
** It permits to make the system fd reference to our non-standard fd, which
** had been opened to reference a file, or a pipe(),  instead of screen or
** keyboard.
**
** An other way to see this :
** an executable is coded to write to an object (screen) which is always pointed
** by fd=1. We can't change the number of the fd which is used by the
** executable, but we can change the object the fd=1 points to.
**
** So instead of pointing to screen object, we can make fd=1 points to a file
** object, or a pipe object.
**
** -----------------------------------------------------------------------------
**
** int	execve(char *path/to/exe, char **args, char **ev);
**
** Then, execution process will use the content of each link as this :
**
** 1 - Parse head->content->command[0] to extract the path & name of the exe
** 2 - If head->content->fd[0], [1], [2]  != 0, 1, 2 -> corresponding
**     standard fd must be redirected with the technique described above
** 3 - Fork a child which will execute the exec with execve();
*/

static int	add_link(t_list **head, t_command *i_command)
{
	t_list		*cmd;

	cmd = ft_lstnew(i_command);
	if (!cmd)
		return (RT_FAIL);
	ft_lstadd_back(head, cmd);
	return (RT_SUCCESS);
}

int	pipeline_n_link(t_list **head, char *execution_line)
{
	char		**pipeline;
	t_command	*i_command;	
	int			fd_tmp;
	int			i;

	pipeline = split_with_exception(execution_line, '|', "\'\"");
	if (!pipeline)
		return (RT_FAIL);
	fd_tmp = -1;
	i = -1;
	while (pipeline[++i])
	{
		i_command = init_command(pipeline[i]);
		if (!i_command)
			return (tokenize_error_pipe(head, pipeline, i, fd_tmp));
		pipe_it(pipeline, i, i_command->fd, &fd_tmp);
		if (add_link(head, i_command) == RT_FAIL)
			return (RT_FAIL);
	}
	free(pipeline);
	return (RT_SUCCESS);
}
