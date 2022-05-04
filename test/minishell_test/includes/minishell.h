#ifndef MINISHELL_H
# define MINISHELL_H
# include "struct.h"
# include "libft.h"
# include "/Users/lecorre/42/part-2/malloc/srcs/includes/malloc.h"
/* # include "/Users/lecorre/Desktop/malloc/includes/malloc.h" */

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/errno.h>
# include <errno.h>
# include <dirent.h>
# ifdef LINUX
#  include <linux/errno.h>
# endif

/* ------------------------------------------------------------------------- */

/*
** Macros
*/
# define MAX_FD 25

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

/*
** builtin functions code magic numbers
*/
# define BT_ECHO 1
# define BT_PWD 2
# define BT_EXIT 3
# define BT_CD 4
# define BT_EXPORT 5
# define BT_UNSET 6
# define BT_ENV 7

/*
** cd error magic numbers
*/
# define HOME_NOT_SET 1
# define OLDPWD_NOT_SET 2
# define INVAL_CD_OPTION 3
# define TOO_MANY_ARGS 4
# define ERRNO_CD 5
# define GETCWD_ERR 6
# define DOUBLE_CD -2
# define CD_FIFO 7

/*
** tokenizer error magic numbers
*/
# define UTOKEN_OR 1
# define UTOKEN_P 2
# define UTOKEN_SC 3
# define UTOKEN_ML 4
# define UTOKEN_RM 5
# define UTOKEN_RL 6
# define UTOKEN_RMM 7
# define UTOKEN_NL 8
# define UTOKEN_DSC 9
# define UTOKEN_HD 10

# define RT_BAD_FD -2

/*
** tokenizer error message
*/
# define E_UTOKEN_OR "OR not handled"
# define E_UTOKEN_P "|"
# define E_UTOKEN_SC ";"
# define E_UTOKEN_ML "multiline not handled"
# define E_UTOKEN_RM ">"
# define E_UTOKEN_RL "<"
# define E_UTOKEN_RMM ">>"
# define E_UTOKEN_NL "newline"
# define E_UTOKEN_DSC ";;"
# define E_UTOKEN_HD "here document not handled"

/*
** Return code
*/
# define RT_FAIL -1
# define RT_SUCCESS 0
# define RT_EXIT 1
# define RT_NOEXIT 2

/*
** Environment control
** These defines shorten literals to make the code more understandable.
** NAME is of type t_list.
# define ENV_KEY(NAME) ((t_env *)((NAME)->content))->key
# define ENV_VALUE(NAME) ((t_env *)((NAME)->content))->value

# define CMD(NAME) ((t_command *)((NAME)->content))->command
# define CMD_FD(NAME) ((t_command *)((NAME)->content))->fd

# define COMMAND(NAME) (t_command *)((NAME)->content)
*/

/*
** Global variables
*/
int	g_line_eraser;
int	g_exit_status;

/* ------------------------------------------------------------------------- */

/*
** These functions will create/add/find/delete/free a list of the t_env struct,
** from the envp (char **ep) argument of the main.
*/
t_list			*create_env_list(char **envp);
char			**env_list_to_tab(t_list *head);
void			add_env_variable(t_list **head, char *var);
char			*find_env_value(t_list **head, char *key);
void			delete_env_variable(t_list **head, char *key);
int				update_underscore(t_list **env, char *path_cmd);
char			*last_arg(t_command *cmd);
long long int	ft_long_long_atoi(const char *str);

/* ------------------------------------------------------------------------- */

/*
** Loop functions
*/
int				main_loop(t_list **env, t_list **export);
int				arg_command(t_list **env, t_list **export, int ac, char **av);
int				gnl_ctrld(int fd, char **line);

/*
** Loop error
*/
int				verify_line(char *line);
int				check_unexpected_token(char *line, int *err);

int				return_to_main(t_list **env, char *line, int ret_gnl,
					 char *saved_path);

/*
** Lexer/parser function that will buid t_command structure
** These functions will create a list of t_command from the input line.
*/
t_list			*tokenizer(char *line);
int				token_error_printer_hub(int err);
int				pipeline_n_link(t_list **head, char *execution_line);
int				expander(t_list **env, t_command *i_command);

/*
** tokenize split
*/
char			**split_with_exception(char *str, char c, char *exception_set);
char			*ghosting(char *str, char c, char *exception_set, int *error);

/*
** tokenize quotes
*/
char			*expand_token(t_list **env, char **line_ptr);
char			*quotes(t_list **env, char **line_ptr);
char			*double_quotes(t_list **env, char **line_ptr);
char			*simple_quotes(char **line_ptr);
char			*no_quotes(t_list **env, char **line_ptr);

int				redirections(t_list **env, char **line_ptr,
					t_command *i_command);

int				expand_doll_quote(t_list **env, char **str, char **final_str,
					 char quote);
char			*doll_expand(t_list **env, char **line_ptr, char quote);

/*
** tokenize quotes utils
*/
int				join_str_before(char **str, char **line_ptr, char **final_str);
int				join_char_to_str(char **final_str, char c);
int				escape_it(char **final_str, char **str, char **line_ptr);

/*
** tokenize error handling
*/
int				pipe_token_error(char **str, int i, int *err);
int				token_pipe(char *line, int i, int space);
int				token_sc(char *line, int i, int space);
int				token_rl(char *line, int i);
int				token_rll(char *line, int i);
int				token_rm(char *line, int i);
int				token_rmm(char *line, int i);

/*
** return error
*/
int				return_expander(t_list **arg, int ret, t_command *cmd);
void			*tokenize_error_sc(t_list **head, char **array, char *line);

int				tokenize_error_pipe(t_list **head, char **pipeline,
					 int i, int fd_tmp);

/*
** tokenize utils
*/
void			delete_remaining_char(char *str, char c);
char			*end_of_object(char *str);
int				is_symbol(int c);
int				is_symbol_doll(int c);
int				is_symbol_v2(int c);
int				authorized_char(int c);
int				is_special_char(int c);
char			*skip_char(char *str, char c);
t_command		*init_command(char *pipeline);

int				ft_count_tab(char **tab);
char			**alpha_order_array(char **export_tab);

/*
** clear lists
*/
void			clear_commandlist(void *content);
void			clear_envlist(void *content);
void			clear_arglist(void *content);
void			clear_arglist_2(void *content);
int				clear_lists_exit(t_list **cmd, t_list **env, char *saved_path);
void			ft_array_string_del(char **array);

/* ------------------------------------------------------------------------- */

//TESTS
int				exec_t(t_list **env, t_command *cmd, char **arr_env);
void			clean_fd_n_wait(int *fd, int cpid);
void			dup_it(int *fd);
void			print_cmd(t_command *cmd);
void			print_array(char **arr);

/*
** This function will find if the command is a builtin and execute it, or
** execute said command with execve.
*/
char			*save_path_env(t_list **env);

int				executer(t_list **env, t_list *cmd,
					t_list **export, char *saved_path);

int				execute_cmd(t_list **env, t_command *cmd,
					t_list **export, char *s_path);
int				execute_command(t_list **env, t_command *cmd, t_list **export);
void			close_fd(int *fd);
void			dup_fd(int *fd);

/*
** Builtins
*/
int				echo_builtin(t_list **env, t_command *cmd);
int				pwd_builtin(t_list **env, t_command *cmd);
int				exit_builtin(t_command *cmd);
int				cd_builtin(t_list **env, t_command *cmd);
int				cd_error(int err, char *path);
char			*check_options(t_list **env, char **cmd, int *fd, int *err);
int				env_builtin(t_list **env, t_command *cmd);
int				export_builtin(t_list **env, t_command *cmd, t_list **export);
void			print_export(char **export_tab, int *fd);
int				unset_builtin(t_list **env, t_command *cmd, t_list **export);
int				is_piped(int *fd);

/*
** Executable external functions
*/

int				execute_builtin(t_list **env, t_command *cmd, int builtin_code,
					t_list **export);
int				is_builtin(t_command *cmd);
int				execute_extern(t_list **env, t_command *cmd, char *saved_path);

char			*path_to_executable(t_list **env, t_command *cmd,
					char *saved_path);
char			*absolute_path(t_command *cmd, char *home_path);

char			*relative_path(t_command *cmd, char **split_path,
					char *saved_path);
char			*test_path_left_right(t_command *cmd, char *saved_path);

void			*test_path_left_right_2(t_command *cmd, char **split,
					int r, int j);
int				test_cmd(char *env_path, char *executable);
void			cmd_found_more_times(t_command *cmd, char **split_path);

/* ------------------------------------------------------------------------- */

/*
** errors/signal handlins/exits functions
*/
void			error_msg(char *bash, t_command *cmd, char *arg, char *err_msg);

int				error_msg_2(char *bash,
					t_command *cmd, char *arg, char *err_msg);
void			ctrl_back_slash_handler(int signal);
void			ctrl_back_slash_handler_quit(int signal);
void			ctrl_c_handler(int sign);
//void			set_line_eraser(int sign);

/* ------------------------------------------------------------------------- */

#endif
