#include "minishell.h"

/*
** open_file() handle file opening thanks to open_code which says which flag
** to give to open & file name 'file', processed just before.
**
** If an error occurs while opening, errno is recorded in fd_command[3], to be
** used at execution time to set the error message. So, even if open() fails,
** processing continues, error just will be displayed after.
*/

static int	open_file(int open_code, int *fd_command, char *file)
{
	if (open_code == 3)
	{
		if (fd_command[0] != 0)
			close(fd_command[0]);
		fd_command[0] = open(file, O_RDONLY);
		if (fd_command[0] == -1)
			fd_command[3] = errno;
		return (1);
	}
	if (fd_command[1] != 1)
		close(fd_command[1]);
	if (open_code == 1)
		fd_command[1] = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else
		fd_command[1] = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd_command[1] == -1)
		fd_command[3] = errno;
	return (1);
}

/*
** redirections() translate the symbol of the first character from the given
** line ('<' , '>') to a code to how to use open().
** It then search for the first character of the string which must be the
** filename, this string will be highly processed by expand_filename() to
** give the expanded string (if necessary) as parameter to open().
** It must expand the dollar character '$', quotes, and also translate tilde.
** Once all data is fetched to use open accordingly, redirections() records the
** given file descriptor in the the appropriate int array which had been given
** from the t_command struct in parameter.
** It modifies the address where the given pointer line_ptr points to the end
** of the processed string.
*/

int	redirections(t_list **env, char **line_ptr, t_command *i_cmd)
{
	int		open_code;
	char	*file;

	if (**line_ptr == '>')
		open_code = 1;
	else
		open_code = 3;
	if (*(*line_ptr + 1) == '>' && ++open_code)
		*line_ptr += 1;
	*line_ptr = skip_char((*line_ptr + 1), ' ');
	file = expand_token(env, line_ptr);
	if (!file)
		return (RT_FAIL);
	open_file(open_code, i_cmd->fd, file);
	if (i_cmd->file)
		free(i_cmd->file);
	i_cmd->file = file;
	if (i_cmd->fd[3] != 0)
		return (RT_BAD_FD);
	return (1);
}
