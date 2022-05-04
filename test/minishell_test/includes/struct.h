#ifndef STRUCT_H
# define STRUCT_H

typedef struct s_command
{
	char		**command;
	char		*unexpanded;
	int			*fd;
	char		*file;
}				t_command;

typedef struct s_env
{
	char		*key;
	char		*value;
}				t_env;

#endif
