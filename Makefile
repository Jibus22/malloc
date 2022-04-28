##### COLORS #####
BLUE = \033[1;35m
GREY = \033[3;31m
GREEN = \033[1;37m
RED = \033[1;31m
END = \033[0m

ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

##### binaries #####
NAME = libft_malloc_$(HOSTTYPE).so
D_NAME = test.out

##### PATH #####
SRCPATH = ./malloc
OBJPATH = $(SRCPATH)/obj
LIBFTPATH = ./libft
TESTPATH = ./test

##### LIB #####
LIBFT = $(LIBFTPATH)/libft.a
LIBSD = -lbsd
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
CCFLAGS = -Wall -Wextra -Werror -std=c90 -fvisibility=hidden
CCTESTFLAGS = -Wall -Wextra -Werror -std=c90

##### OSTYPE #####
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	# mac
	CCFLAGS += -D DARWIN
else
	#Linux and others...
	CCFLAGS += -D LINUX
endif

##### D_SRCS #####
D_SRCS = $(addprefix $(TESTPATH)/, main.c)

##### SRCS #####
SRCS = $(addprefix $(SRCPATH)/, malloc.c show_alloc_mem.c free.c realloc.c)

OBJ = $(SRCS:$(SRCPATH)/%.c=$(OBJPATH)/%.o)


### RULES ###

all : mk_objdir $(NAME)


mk_objdir:
	@if [ ! -d $(OBJPATH) ]; then mkdir $(OBJPATH); fi


$(NAME): $(OBJ) $(LIBFT)
	@echo "\n$(END)$(BLUE)# Making $(NAME) #$(END)$(GREY)"
	$(CC) -shared -o $@ $(OBJ) $(LIBFT)
	@echo "\n$(END)$(GREEN)# $(NAME) is built #$(END)"

$(LIBFT): 
	@Make -C $(LIBFTPATH)

$(OBJPATH)/%.o : $(SRCPATH)/%.c $(HEADERS)
	$(CC) $(CCFLAGS) $(INC) -c $< -o $@

test: all
	$(CC) $(CCTESTFLAGS) $(INC) $(D_SRCS) -L. -l$(LIBMALLOC) -o $(TESTPATH)/$(D_NAME)
	@echo "\n$(END)$(GREEN)# $(D_NAME) is built #$(END)"

runtest: test
	@echo "\n$(END)$(GREEN)# $(D_NAME) #### [RUN] ####$(END)"
	@./$(TESTPATH)/$(D_NAME)

help:
	@echo "  test:    build test.out\n\
	  runtest: build test.out and run it"

### CLEAN ###
.PHONY : sanitize clean fclean re

clean :
	@echo "$(END)$(RED)# removing $(NAME) objects #$(END)$(GREY)"
	@rm -rf $(OBJ)
	@make clean -C $(LIBFTPATH)

fclean : clean
	@echo "$(END)$(RED)\n# removing $(NAME) #$(END)$(GREY)"
	@rm -f $(NAME)
	@rm -f $(TESTPATH)/$(D_NAME)
	@make fclean -C $(LIBFTPATH)

re : fclean all

