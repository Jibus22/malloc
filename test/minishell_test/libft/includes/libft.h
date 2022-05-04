/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/24 10:38:00 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/15 20:51:31 by whoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

/*
** Libaries
*/
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/uio.h>

/*
** Macros
*/
# define MAX_FD 25

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

/*
** structure bonus libft
*/
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

int				ft_atoi(char const *str);
void			ft_bzero(void *s, size_t n);
void			*ft_calloc(size_t count, size_t size);
int				ft_intlen_base(unsigned long n, int base);
size_t			ft_intlen(int n);
int				ft_isalnum(int c);
int				ft_isalpha(int c);
int				ft_isascii(int c);
int				ft_isdigit(int c);
int				ft_isprint(int c);
char			*ft_itoa(int n);
void			*ft_memalloc(size_t size);
void			*ft_memccpy(void *dst, void const *src, int c, size_t n);
void			*ft_memchr(const void *s, int c, size_t n);
int				ft_memcmp(void const*s1, void const *s2, size_t n);
void			*ft_memcpy(void *dst, void const *src, size_t n);
void			ft_memdel(void **ap);
void			*ft_memmove(void *dst, void const *src, size_t len);
void			*ft_memset(void *b, int c, size_t len);
void			ft_putchar(char c);
void			ft_putchar_fd(char c, int fd);
void			ft_putendl(char const *s);
void			ft_putendl_fd(char *s, int fd);
void			ft_putnbr_fd(int n, int fd);
void			ft_putstr(char const *s);
void			ft_putstr_fd(char *s, int fd);
void			*ft_realloc(void *ptr, size_t size);
void			*ft_freetab(char **tab);
char			*ft_strcat(char *s1, char const *s2);
char			*ft_strchr(char const *s, int c);
char			*ft_strcpy(char *dest, char const *src);
void			ft_strdel(char **as);
char			*ft_strdup(char const *s1);
int				ft_strequ(char const *s1, char const *s2);
void			ft_striter(char *s, void (*f)(char*));
void			ft_striteri(char *s, void (*f)(unsigned int, char*));
char			*ft_strjoin(char const *s1, char const *s2);
size_t			ft_strlcat(char *dst, char const *src, size_t dstsize);
size_t			ft_strlcpy(char *dst, char const *src, size_t dstsize);
int				ft_strcmp(char const *s1, char const *s2);
size_t			ft_strclen(char const *s, int c);
void			ft_strclr(char *s);
size_t			ft_strlen(char const *s);
char			*ft_strmap(char const *s, char (*f)(char));
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char			*ft_strncat(char *s1, char const *s2, size_t n);
int				ft_strncmp(char const *s1, char const *s2, size_t n);
char			*ft_strncpy(char *dest, char const *src, size_t len);
int				ft_strnequ(char const *s1, char const *s2, size_t n);
char			*ft_strnew(size_t size);
char			*ft_strnstr(char const *s, char const *needle, size_t l);
char			*ft_strrchr(char const *s, int c);
char			*ft_strstr(char const *haystack, char const *needle);
char			*ft_strtrim(char const *s1, char const *set);
char			*ft_substr(char const *s, unsigned int start, size_t len);
int				ft_tolower(int c);
int				ft_toupper(int c);
int				ft_islower(int c);
int				ft_isupper(int c);
/*
** jb
*/
char			**ft_split_jb(char const *s, char c);
int				join_newstr(char **str, const char *src);
int				get_next_line(int fd, char **line);
void			ft_lstadd_back(t_list **alst, t_list *new);
void			ft_lstadd_front(t_list **alst, t_list *new);
t_list			*ft_lstlast(t_list *lst);
t_list			*ft_lstnew(void *content);
int				ft_lstsize(t_list *lst);
void			ft_lstiter(t_list *lst, void (*f)(void *));
void			ft_lstclear(t_list **lst, void (*del)(void *));
void			ft_lstdelone(t_list *lst, void (*del)(void *));
t_list			*ft_lstmap(t_list *lst, void *(*f)(void*), void (*del)(void*));

#endif
