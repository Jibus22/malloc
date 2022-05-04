# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/06/24 10:43:20 by jfreitas          #+#    #+#              #

#    Updated: 2021/02/22 19:25:59 by jle-corr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CC = clang
CFLAGS = -Wall -Wextra -Werror
INCPATH = includes/
DEPS = $(INCPATH)/libft.h

OBJDIR = srcs/libftobj

SRCS =	srcs/ft_atoi.c \
		srcs/ft_bzero.c \
		srcs/ft_calloc.c \
		srcs/ft_intlen_base.c \
		srcs/ft_intlen.c \
		srcs/ft_isalnum.c \
		srcs/ft_isalpha.c \
		srcs/ft_isascii.c \
		srcs/ft_isdigit.c \
		srcs/ft_isprint.c \
		srcs/ft_itoa.c \
		srcs/ft_memalloc.c \
		srcs/ft_memccpy.c \
		srcs/ft_memchr.c \
		srcs/ft_memcmp.c \
		srcs/ft_memcpy.c \
		srcs/ft_memdel.c \
		srcs/ft_memmove.c \
		srcs/ft_memset.c \
		srcs/ft_putchar.c \
		srcs/ft_putchar_fd.c \
		srcs/ft_putendl.c \
		srcs/ft_putendl_fd.c \
		srcs/ft_putnbr.c \
		srcs/ft_putnbr_fd.c \
		srcs/ft_putstr.c \
		srcs/ft_putstr_fd.c \
		srcs/ft_realloc.c \
		srcs/ft_strcat.c \
		srcs/ft_strchr.c \
		srcs/ft_strclen.c \
		srcs/ft_strclr.c \
		srcs/ft_strcmp.c \
		srcs/ft_strcpy.c \
		srcs/ft_strdel.c \
		srcs/ft_strdup.c \
		srcs/ft_strequ.c \
		srcs/ft_striter.c \
		srcs/ft_striteri.c \
		srcs/ft_strjoin.c \
		srcs/ft_strlcat.c \
		srcs/ft_strlen.c \
		srcs/ft_strmap.c \
		srcs/ft_strmapi.c \
		srcs/ft_strncat.c \
		srcs/ft_strncmp.c \
		srcs/ft_strncpy.c \
		srcs/ft_strnequ.c \
		srcs/ft_strnew.c \
		srcs/ft_strnstr.c \
		srcs/ft_strrchr.c \
		srcs/ft_strstr.c \
		srcs/ft_substr.c \
		srcs/ft_strtrim.c \
		srcs/ft_tolower.c \
		srcs/ft_toupper.c \
		srcs/ft_intlen.c \
		srcs/ft_islower.c \
		srcs/ft_isupper.c 


SRCS += $(addprefix srcs/, ft_lstadd_back_jb_bonus.c ft_lstdelone_jb_bonus.c \
		ft_lstmap_jb_bonus.c ft_lstadd_front_jb_bonus.c ft_lstiter_jb_bonus.c \
		ft_lstnew_jb_bonus.c ft_lstclear_jb_bonus.c ft_lstlast_jb_bonus.c \
		ft_lstsize_jb_bonus.c get_next_line.c ft_split_jb.c ft_freetab.c \
		join_newstr.c)

OBJ = $(SRCS:srcs/%.c=$(OBJDIR)/%.o)


#RULES
all: mk_objdir $(NAME)

mk_objdir :
	@if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi

$(OBJDIR)/%.o : srcs/%.c $(DEPS)
	@$(CC) $(CFLAGS) -I $(INCPATH) -c $< -o $@

$(NAME): message $(OBJ)
	@echo "\033[1;36mBuilding $@\033[0m"
	@ar rcs $@ $(OBJ)
	@echo "\033[1;36m$@ is built !\033[0m"

message :
	@echo "\n\033[1;36mBuilding $(NAME) objects\033[0m"

#CLEAN
clean:
	@rm -rf $(OBJ)
	@echo "\033[1;33m$(NAME) objects removed.\033[0m"

fclean: clean
	@rm -f $(NAME)
	@echo "\033[1;33m$(NAME) removed.\033[0m"

re: fclean all
