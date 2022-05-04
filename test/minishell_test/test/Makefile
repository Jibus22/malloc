NAME = testit


##### COLORS #####
BLUE = \033[3;37m
GREY = \033[3;31m
GREEN = \033[1;32m
RED = \033[3;35m
END = \033[0m


##### SRC & OBJ PATH #####
LIBFTPATH = ../libft
SRC_TEST_PATH = .
SRC_PATH = ../srcs
OBJ_TEST_PATH = $(SRC_TEST_PATH)/obj
OBJ_PATH = $(SRC_PATH)/obj_test

##### INCLUDE #####
INC_MINISHELL = ../includes
INC_LIBFT = $(LIBFTPATH)/includes

##### INCLUDES DEPENDENCIES ####
INC_DEP = $(INC_MINISHELL)/*.h

##### LIB #####
LIBFT = $(LIBFTPATH)/libft.a

##### FLAG #####
CC = clang
FLAGS = -Wall -Wextra -Werror -g

##### OSTYPE #####
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	# mac
	FLAGS += -D DARWIN
else
	#Linus and others...
	FLAGS += -D LINUX
endif



##### SRCS FOR TESTING PURPOSE #####
################################################################################
# add your own main_test & test files if needed, from ./test folder, in this one
SRCS_TEST = $(addprefix $(SRC_TEST_PATH)/, loop_test.c exec_t.c)
# below, add program files from ./srcs folder, that u wanna test with your main
# test added above, in this one. It can be empty
SRCS_PGM = $(addprefix $(SRC_PATH)/,builtins_1.c builtins_2.c environment.c \
		   environment_2.c executable_builtin.c signal_handler.c \
		   exit_error_manager.c tokenizer.c token_find_redirections.c \
		   token_utils.c token_quotes.c token_expand.c token_spliters.c \
		   token_redirections.c clear_lists.c main_loop.c builtins_3.c \
		   file_descriptor.c utils.c executable_external_path.c \
		   executable_external.c)




##### OBJS #####
OBJ = $(SRCS_TEST:$(SRC_TEST_PATH)/%.c=$(OBJ_TEST_PATH)/%.o)
OBJ += $(SRCS_PGM:$(SRC_PATH)/%.c=$(OBJ_PATH)/%.o)


### RULES ###

all : mk_objdir mk_libft $(NAME)


mk_objdir:
	@if [ ! -d $(OBJ_PATH) ]; then mkdir $(OBJ_PATH); fi
	@if [ ! -d $(OBJ_TEST_PATH) ]; then mkdir $(OBJ_TEST_PATH); fi

mk_libft :
	@echo "\n$(END)$(BLUE)Checking libft$(END)"
	@make -C $(LIBFTPATH)


$(NAME) : message $(OBJ) $(INC_DEP)
	@echo "\n$(END)$(BLUE)Making $(NAME)$(END)$(GREY)"
	$(CC) -o $@ $(OBJ) $(LIBFT)
	@echo "\n$(END)$(GREEN)$(NAME) is built$(END)"
	@cp $(NAME) ..

message :
	@echo "\n$(END)$(BLUE)Making $(NAME) objects$(END)$(GREY)"


$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c $(INC_DEP)
	$(CC) $(FLAGS) -I $(INC_LIBFT) -I $(INC_MINISHELL) -c $< -o $@

$(OBJ_TEST_PATH)/%.o : $(SRC_TEST_PATH)/%.c $(INC_DEP)
	$(CC) $(FLAGS) -I $(INC_LIBFT) -I $(INC_MINISHELL) -c $< -o $@

#CLEAN
.PHONY : clean fclean re

clean :
	@echo "$(END)$(RED)\nremoving $(NAME) objects$(END)$(GREY)"
	rm -rf $(OBJ)
	@echo "$(END)$(RED)cleaning libft objects$(END)"
	@make clean -C $(LIBFTPATH)

fclean : clean
	@echo "$(END)$(RED)\nremoving $(NAME)$(END)$(GREY)"
	@rm -f ../$(NAME); rm -f $(NAME)
	@echo "$(END)$(RED)\nremoving libft.a$(END)"
	@make fclean -C $(LIBFTPATH)

re : fclean all
