##### COLORS #####
BLUE = \033[1;35m
GREY = \033[3;31m
GREEN = \033[1;37m
RED = \033[1;31m
END = \033[0m

ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

##### NAMES #####
NAME = libft_malloc_$(HOSTTYPE).so
L_NAME = libft_malloc.so
D_NAME = test.out

##### PATH #####
SRCPATH = ./srcs
OBJPATH = $(SRCPATH)/obj
LIBFTPATH = ./libft
TESTPATH = ./test

##### LIBS #####
LIBFT = $(LIBFTPATH)/libft.a
LIBMALLOC = ft_malloc_$(HOSTTYPE)

##### INCLUDE #####
PATH_INCLUDE = $(SRCPATH)/includes
PATH_INCLUDE2 = ./libft
HEADERS = $(PATH_INCLUDE)/*.h
HEADERS2 = $(PATH_INCLUDE2)/*.h
INC = $(addprefix -I , $(PATH_INCLUDE) $(PATH_INCLUDE2))


##### COMPILER #####
CC = clang
##### COMPILATION FLAG #####
CCFLAGS = -Wall -Wextra -Werror -fPIC -fvisibility=hidden
CCTESTFLAGS = -Wall -Wextra -Werror

##### OSTYPE #####
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	# mac
	CCFLAGS += -D DARWIN
else
	#Linux and others...
	CCFLAGS += -D LINUX
endif

##### SRCS #####
SRCS = $(addprefix $(SRCPATH)/, malloc.c show_alloc_mem.c free.c realloc.c\
			 calloc.c shared.c constructor.c destructor.c shared_log.c)

OBJ = $(SRCS:$(SRCPATH)/%.c=$(OBJPATH)/%.o)


### RULES ###

### BUILD ###

all : mk_objdir $(NAME)


mk_objdir:
	@if [ ! -d $(OBJPATH) ]; then mkdir $(OBJPATH); fi


$(NAME): $(LIBFT) $(OBJ)
	@echo "\n$(END)$(BLUE)# Making $(NAME) #$(END)$(GREY)"
	$(CC) -shared -o $@ $(OBJ) $(LIBFT)
	@ln -sf $@ $(L_NAME)
	@echo "\n$(END)$(GREEN)# $(NAME) is built #$(END)"

$(LIBFT): 
	@make -C $(LIBFTPATH)

$(OBJPATH)/%.o : $(SRCPATH)/%.c $(HEADERS)
	$(CC) $(CCFLAGS) $(INC) -c $< -o $@

### TESTS ###
home_tests: all
	@sh test/my_tests.sh

regular_tests: all
	@sh test/correction/correction.sh

timed_regular_tests: all
	@sh test/correction/correction.sh -time

minishell: all
	@sh test/minishell_test/minishell.sh

help:
	@echo "----- RULES: -----\n\n"\
		"      home_tests:    run my_tests.sh in test/\n"\
		"   regular_tests:    run correction.sh in test/correction/\n"\
		"       minishell:    compile and run test/minishell/./minishell with lib\n"\
		"-----------------"

### CLEAN ###
.PHONY : sanitize clean fclean re

clean :
	@echo "$(END)$(RED)# removing $(NAME) objects #$(END)$(GREY)"
	@rm -rf $(OBJ)
	@make clean -C $(LIBFTPATH)

fclean : clean
	@echo "$(END)$(RED)\n# removing $(NAME) #$(END)$(GREY)"
	@rm -f $(NAME)
	@rm -f $(L_NAME)
	@rm -f $(TESTPATH)/$(D_NAME)
	@make fclean -C $(LIBFTPATH)

re : fclean
	@$(MAKE)
