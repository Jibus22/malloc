#include "minishell.h"

int	line_eraser;

void		print_array2(char **array)
{
        int                     i;

        i = -1;
		//printf(LINE(PRINT_LIST));
        while (array[++i])
                printf("array[%d] :|%s|\n", i, array[i]);
		printf("array[%d] :|%s|\n", i, array[i]);
        printf(LINE2);
}

void		print_fd(int *fd)
{
	printf("fd[0] = %d\tfd[1] = %d\tfd[2] = %d\n", fd[0], fd[1], fd[2]);
	printf(LINE2);
}

void		print_list(t_list *cmd)
{
	while (cmd)
	{
		print_array2(((t_command*)(cmd->content))->command);
		print_fd(((t_command*)(cmd->content))->fd);
		cmd = cmd->next;
		printf(LINE2);
	}
}

void		display_promptt(int sign)
{
	if (sign == SIGINT)
	{
		ft_putstr("\nminishell-1.0$ ");
		fflush(stdout);
	}
}

void		set_line_eraserr(int sign)
{
	if (sign == SIGINT)
	{
		line_eraser = 1;
		ft_putstr("\nminishell-1.0e$ ");
		fflush(stdout);
	}
}

/*
** if the char is 'n' instead of '\n' :
** if I type '\n', it writes a newline. But if I type 'n', many times
** and continue after, it does nothing UNTIL I type '\n'.
** that's mean that the 'enter' key sends a signal to read() saying :
** "now you read all that has been wrote to the stdin !". Like when a fifo
** write end fd is closed (which says to read end that it can read the buffer)
**
** We could say that keyboard & read(fd0) are linked by a fifo buffer (pipe)
** & that enter key closes the write end fd.
** All characters typed after n aren't displayed bc my gnl puts a
** 0 at the place of flag character.
**
** If I type '\n' before typing 'n', it just writes '\n', BUT, we can't backspace
** it. That means that read() has read. But since 'n' hasn't been met, line
** is still full and gnl didn't returned
** So, when it's a '\n' character' inside gnl, it sends signal to read() to read
** plus returning gnl has he met '\n'
** 
** ctrl^D sends EOT signal where read() then returns 0. So it says to read()
** not to read 0 Bytes but to read the fifo buffer then returns 0.
*/

/*
** We enter into check_ctrld when read returns 0, so When ctrl^D or ctrl^C is
** typed.
**
** ctrl^D after inputing  : line exists -read() had read and gnl joined buf to line
** ctrl^D with empty input: exit read() & so the minishell.
** ctrl^C 				  : line inputed before doesn't exists bc read() hadn't
**							read. If inputed ctrl^D had been hited before, a line
**							exists.
**
** When ctrl^D is hited, 2 behaviors :
**				- If Nothing had already been typed, exit the loop (return 0).
**				- else we want to stay into the read() loop to continue to
**				creates the line : don't return.
**
** When ctrl^C is hited, 1 behavior :
**				- Display a new prompt, line erased.
**
** If ctrl^D has been typed, signal() into check_ctrld will take advantage.
** Therefore, the ctrl^C signal handler called into this func has to set global
** variable line_eraser to 1, to say to the loop to erase the line.
**
** When ctrl^C is typed after a ctrl^D this is signal() inside check_ctrld()
** which catch it although we don't enter again in this func(). The only moment
** when we go into it is when ctrl^D is hited.
** But when ctrl^D is hited & read() continues, this signal() takes effect and
** takes advantage and will act when ctrl^C will be hited, until we go out of
** gnl which would have returned 1, then the more general signal() go back
*/

int		check_ctrldd(char **line)
{
	signal(SIGINT, set_line_eraserr);
	if (**line && line_eraser == 0)
		return (1);
	else
		return (0);
}

void	eraser_checkerr(char *line)
{
	if (line_eraser == 1)
	{
		ft_memset(line, 0, ft_strlen(line));
		line_eraser = 0;
	}
}

/*
** 'ENTER':							~> fifo write end closed(?). read() can read
**									   the fifo buffer, then wait to read again.
** ctrl^D : read(fd0) -> returns 0	~> EOT. fifo write end closed and read()
**									   returns 0, buffer looks like being
**									   fullfiled anyway, which means read() act.
** ctrl^C : no read() -> no return  ~> sigint. As signal() is used, pgm doesn't
**									   interrupt & currently print a new prompt,
**									   but still interrupt read without any read
**									   of buffer. No read() at all from the fifo
**									   buffer. read() returns 0 anyway.
**									   So, ctr^C handler must erase the line
**									   and display a new prompt.
**
**
**		(ctrl^D)-,       ,-(ctrl^C)
**               v       v
** > "hello      \0"world  
** > Bla
**
** Old behavior       ----->line--->|helloBla|
** what we should have----->line--->|Bla|
**
** When ctrl^C is sent, line is empty because if it exists it has been erased.
*/

int		gnl_hacked(int fd, char **line)
{
	static char		buf[MAX_FD][BUFFER_SIZE + 1];
	char			*adr;

	if (BUFFER_SIZE < 1 || fd < 0 || !line ||
			fd > MAX_FD || read(fd, buf[fd], 0) == -1)
		return (-1);
	if (!(*line = ft_strnew(0)))
		return (0);
	signal(SIGINT, display_promptt);
	while (!(adr = ft_strchr(buf[fd], '\n')))
	{
		if (!(join_newstr(line, buf[fd])))
			return (-1);
		ft_memset(buf[fd], 0, BUFFER_SIZE);
		if (!(read(fd, buf[fd], BUFFER_SIZE)))
			if (!check_ctrldd(line))
				return (0);
		eraser_checkerr(*line);
	}
	*adr = 0;
	if (!(join_newstr(line, buf[fd])))
		return (-1);
	ft_strncpy(buf[fd], adr + 1, sizeof(buf[fd]));
	return (1);
}

int			main(int ac, char *av[], char *ep[])
{
	char	*line;
	t_list	*cmd;
	t_list	*cmd_cp;
	t_list	*env;
	int		ret;
	char	**arr_env;

	line_eraser = 0;
	env = create_env_list(ep);
	arr_env = env_list_to_tab(env);
	ft_putstr("minishell-1.0$ ");
	while ((ret = gnl_hacked(0, &line)) > 0)
	{
		printf("this shit has to be parsed: %s\n", line);
		//printf("BEFORE TOK\n");
		cmd = tokenize_line_jb(line, &env);
		//printf("\nAFTER TOK\n");
		free(line);
		//print_list(cmd);
		cmd_cp = cmd;
		while (cmd_cp)
		{
		/*	printf(LINE(PRINT_LINK));
			print_array2(((t_command*)(cmd_cp->content))->command);
			print_fd(((t_command*)(cmd_cp->content))->fd);*/
			errno = 0;
			exec_t(&env, (t_command*)(cmd_cp->content), arr_env);
			//execute_command(&env, (t_command*)(cmd_cp->content));
			cmd_cp = cmd_cp->next;
			/*printf(LINE2);
			printf(LINE2);
			printf(LINE2);*/
		}
		//print_list(cmd);
		ft_lstclear(&cmd, &clear_commandlist);
		ft_putstr("minishell-1.0$ ");
	}
	free(line);
	ft_lstclear(&env, &clear_envlist);
	ft_freetab(arr_env);
	if (ret == -1)
		return (-1);
	if (ret == 0)
		ft_putstr("exit\n");
	//system("leaks testit");
	if (ac && av && ep)
		;
	return (0);
}
