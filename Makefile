CC = cc
CFLAGS = -Wall -Werror -Wextra -g -Iinclude
NAME = minishell

LIBFT_DIR = libft
LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline
LIBFT = $(LIBFT_DIR)/libft.a

SRC_DIR = src
OBJS_DIR = objs

SRC =  	parsing/parsing.c \
		parsing/splitter.c \
		parsing/identify_token.c \
		parsing/syntax.c \
		parsing/preprocess_input.c \
		main.c init.c\
		signals.c \
		exit_handler.c \
		execution/execute_pipeline.c \
		execution/exec_in_child.c \
		execution/build_args.c \
		execution/find_cmd.c \
		execution/redirections.c \
		execution/exec_utils.c \
		execution/exec_in_parent.c \
		execution/exec_builtins.c \
		execution/error_checks.c \
		execution/heredoc.c \
		execution/execute_cmd_handler.c \
		envp/envp_errors.c \
		envp/envp_to_str.c \
		envp/envp_utils.c \
		envp/ft_getenv.c  \
		envp/ft_setenv.c \
		envp/init_envp.c \
		envp/shlvl.c \
		envp/sort_envp.c \
		utils/list_utils.c \
		utils/utils.c \
		utils/utils_spaces.c \
		utils/utils_qoutes.c \
		builtin_functions/cd.c \
		builtin_functions/echo.c \
		builtin_functions/env.c \
		builtin_functions/export/export.c \
		builtin_functions/export/export_syntax.c \
		builtin_functions/export/export_utils.c \
		builtin_functions/export/export_set.c \
		builtin_functions/pwd.c \
		builtin_functions/unset.c \
		builtin_functions/exit.c \
		expander/expander.c \
		expander/expand_env.c \
		expander/expand_arg.c \
		expander/expand_utils.c \
		expander/cleanup_input.c \
		expander/replace_tokens.c \
		expander/strip_quotes.c \
		expander/process_env_token.c \
		expander/expand_env_helpers.c \
		expander/expand_cleanup_tokens.c \
# Automatically find the full paths of the source files
SRCS = $(addprefix $(SRC_DIR)/, $(SRC))
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJS_DIR)/%.o, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# Rule to create .o files while preserving the subdirectory structure
$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJS_DIR)
	@mkdir -p $(dir $@)  # Ensure subdirectories exist
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJS_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
