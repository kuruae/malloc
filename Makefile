RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
RESET = \033[0m

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
LINK_NAME = libft_malloc.so

CC = clang
CFLAGS = -Wall -Wextra -Werror -fPIC -std=c11
INC = -I./includes -I./libft/includes

SRC_DIR = srcs
OBJ_DIR = objs
LIBFT_PATH = libft

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

INC_DIRS = $(INC)

LIBFT = $(LIBFT_PATH)/libft.a

all: $(NAME)

$(LIBFT):
	@printf "Building libft...\n"
	@make -C $(LIBFT_PATH) > /dev/null 2>&1
	@printf "$(GREEN)libft built successfully!$(RESET)\n"

$(NAME): $(LIBFT) $(OBJ_FILES)
	@$(CC) $(CFLAGS) -shared -o $(NAME) $(OBJ_FILES) -L$(LIBFT_PATH) -lft
	@ln -sf $(NAME) $(LINK_NAME)
	@printf "$(GREEN)$(NAME) built and linked.\n$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INC_DIRS) -c $< -o $@
	@printf "$(YELLOW)Compiling $<$(RESET)\n"

clean:
	@make -C $(LIBFT_PATH) clean > /dev/null 2>&1
	@printf "Cleaning object files...\n"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@make -C $(LIBFT_PATH) fclean > /dev/null 2>&1
	@printf "Removing $(NAME) and $(LINK_NAME)...\n"
	@rm -f $(NAME) $(LINK_NAME)

re: fclean all

clangd: clean
	@which bear >/dev/null 2>&1 || { printf "$(RED)Error: bear not installed.$(RESET)"; exit 1; }
	@mkdir -p $(OBJ_DIR)
	@printf "$(GREEN)Generating compile_commands.json with bear...$(RESET)\n"
	@bear -- $(MAKE) all
	@printf "$(GREEN)Done. compile_commands.json ready for clangd.$(RESET)"

test: all
	@$(CC) tests/main_test.c -L. -lft_malloc -Iincludes -o test_prog
	@echo "Testing with LD_PRELOAD..."
	@export LD_LIBRARY_PATH=$$(pwd); \
	 export LD_PRELOAD=$$(pwd)/libft_malloc.so; \
	 ./test_prog

.PHONY: all clean fclean re clangd
