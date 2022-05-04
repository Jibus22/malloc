#include "minishell.h"

/*
** If the char is 'n' instead of '\n':
** if I type '\n', it writes a newline. But if I type 'n', many times
** and continue after, it does nothing UNTIL I type '\n'.
** that means that the 'enter' key sends a signal to read() saying:
** "now you read all that has been wrote to the stdin!". Like when a fifo
** write end fd is closed (which says to read end that it can read the buffer).
**
** We could say that keyboard & read(fd0) are linked by a fifo buffer (pipe)
** & that enter key closes the write end fd.
** All characters typed after n aren't displayed because my gnl puts a 0 at
** the place of flag character.
**
** If I type '\n' before typing 'n', it just writes '\n', BUT, we can't
** backspace it. That means that read() has read. But since 'n' hasn't been met,
** line is still full and gnl didn't returned.
** So, when it's a '\n' character inside gnl, it sends a signal to read() to
** read, plus returning gnl has the met '\n'.
**
**
** ctrl^D sends EOT signal where read() then returns 0. So it says to read()
** not to read 0 Bytes but to read the fifo buffer then returns 0.
**
**
** We enter into check_ctrld when read returns 0, so when ctrl^D or ctrl^C is
** typed.
**
** ctrl^D after input: line exists -read() has read and gnl joined buf to line.
** ctrl^D with empty input: exit read() & so the minishell.
** ctrl^C: line inputed before doesn't exists because read() hadn't read.
** If inputed ctrl^D had been hited before, a line exists.
**
** When ctrl^D is hited, 2 behaviors:
**				- If Nothing had already been typed, exit the loop (return 0).
**				- else we want to stay into the read() loop to continue to
**				  create the line : don't return.
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
** gnl which would have returned 1, then the more general signal() goes back.
*/

static void	eraser_checker(char *line)
{
	if (g_line_eraser == 1)
	{
		g_exit_status = 130;
		ft_memset(line, 0, ft_strlen(line));
		g_line_eraser = 0;
	}
}

static int	check_ctrld(char **line)
{
	eraser_checker(*line);
	if (**line && g_line_eraser == 0)
	{
		return (1);
	}
	else if (!**line)
	{
		ft_putstr_fd("exit\n", 2);
		return (0);
	}
	return (1);
}

/*
** 'ENTER':						   ~> fifo write end closed(?). read() can read
**									  the fifo buffer, then wait to read again.
** ctrl^D : read(fd0) -> returns 0 ~> EOT. fifo write end closed and read()
**									  returns 0, buffer looks like being
**									  fullfiled anyway, which means read() act.
** ctrl^C : no read() -> no return ~> SIGINT. As signal() is used, pgm doesn't
**									  interrupt & currently print a new prompt,
**									  but still interrupt read without any read
**									  of buffer. No read() at all from the fifo
**									  buffer. read() returns 0 anyway.
**									  So, ctr^C handler must erase the line
**									  and display a new prompt.
**
**
**			(ctrl^D)-,       ,-(ctrl^C)
**                   v       v
** > "hello          \0"world
** > Bla
**
** Old behavior       ----->line--->|helloBla|
** what we should have----->line--->|Bla|
**
** When ctrl^C is sent, line is empty because if it exists it has been erased.
*/

static void	display_prompt(void)
{
	if (g_line_eraser == 0)
		ft_putstr_fd("\033[1;32mminishell$\033[0m ", 1);
}

int	gnl_ctrld(int fd, char **line)
{
	static char		buf[MAX_FD][BUFFER_SIZE + 1];
	char			*adr;

	if (BUFFER_SIZE < 1 || fd < 0 || !line
		|| fd > MAX_FD || read(fd, buf[fd], 0) == -1)
		return (-1);
	signal(SIGINT, ctrl_c_handler);
	display_prompt();
	*line = ft_strnew(0);
	adr = NULL;
	while (!adr)
	{
		join_newstr(line, buf[fd]);
		ft_memset(buf[fd], 0, BUFFER_SIZE);
		if (!(read(fd, buf[fd], BUFFER_SIZE)))
			if (!check_ctrld(line))
				return (0);
		eraser_checker(*line);
		adr = ft_strchr(buf[fd], '\n');
	}
	*adr = 0;
	join_newstr(line, buf[fd]);
	ft_strncpy(buf[fd], adr + 1, sizeof(buf[fd]));
	return (1);
}
