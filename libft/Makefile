# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/11 04:58:03 by jle-corr          #+#    #+#              #
#    Updated: 2022/05/12 16:11:04 by jle-corr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = clang
CFLAGS = -Wall -Wextra -Werror
DEPS = libft.h
OBJDIR = libftobj

SRC = ft_atoi.c ft_bzero.c ft_isalnum.c ft_isalpha.c ft_isascii.c \
		ft_isdigit.c ft_isprint.c ft_memccpy.c ft_memchr.c \
		ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c \
		ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c \
		ft_strchr.c ft_strlcat.c \
		ft_strlcpy.c ft_strlen.c  ft_strncmp.c \
		ft_strnstr.c ft_strrchr.c \
		ft_tolower.c ft_toupper.c ft_pow.c ft_putstr.c   \
		ft_strncpy.c \


OBJ = $(SRC:%.c=$(OBJDIR)/%.o)
NAME = libft.a

all : mk_objdir $(NAME)

mk_objdir :
	@if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi

$(OBJDIR)/%.o : %.c $(DEPS)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME) : message $(OBJ)
	@ar rcs $@ $(OBJ)
	@echo "\033[1;36m$@ is built !\033[0m"

message :
	@echo "\n\033[1;36mBuilding $(NAME) objects\033[0m"

.PHONY : clean
clean :
	@rm -rf $(OBJ)
	@echo "\033[1;33m$(NAME) objects removed.\033[0m"

fclean : clean
	@rm -f $(NAME)
	@echo "\033[1;33m$(NAME) removed.\033[0m"

re : fclean all
