# **************************************************************************** #
#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/29 08:20:09 by cberganz          #+#    #+#              #
#    Updated: 2022/02/27 02:26:37 by cberganz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME		= minishell

NAME_BONUS	= minishell_bonus

INCLUDE		= include/

HEADER		= $(INCLUDE)minishell.h

HEADER_BONUS= $(INCLUDE)minishell_bonus.h

CC			= clang

CFLAGS		= -Wall -Wextra -Werror -g

SRC_FILES 	=	minishell.c				\
				terminal/loop.c			\
				terminal/prompt.c		\
				utils/utils_signals.c	\
				utils/utils_print.c		\
				utils/utils_exit.c		\
				utils/utils_env.c		\
				utils/utils_parsing.c	\
				utils/utils_close_fds.c	\
				utils/utils_bonus.c		\
				parsing/errors_token.c	\
				parsing/parse_parenthesis.c\
				parsing/errors_not_handled.c\
				parsing/parse_step0_comments.c\
				parsing/parse_step1.c	\
				parsing/parse_step2.c	\
				parsing/parse_step3_variable.c\
				parsing/parse_step4_splitargs.c\
				parsing/parse_step5_removequotes.c\
				parsing/parse_global.c	\
				parsing/redirections_step1.c\
				parsing/redirections_step2.c\
				parsing/redirections_step3.c\
				parsing/wildcard.c		\
				parsing/redirections_heredoc_loop.c\
                parsing/redirections_heredoc_variables.c\
				exec/exec_main.c		\
				exec/exec_bin.c			\
				exec/exec_builtin.c		\
				exec/forking.c			\
				exec/get_path.c			\
				builtin/builtin_exit.c	\
				builtin/builtin_echo.c	\
				builtin/builtin_pwd.c	\
				builtin/builtin_env.c	\
				builtin/builtin_cd.c	\
				builtin/builtin_export_1.c\
				builtin/builtin_export_2.c\
				builtin/builtin_export_3.c\
				builtin/builtin_unset.c\
				utils/utils_prompt_error.c

SRC_DIR		= src/
SRC			= $(addprefix $(SRC_DIR), $(SRC_FILES))

OBJ_DIR		= objs/
OBJ_DIRS	= $(sort $(dir $(OBJ)))
OBJ_FILES	= $(SRC_FILES:.c=.o)
OBJ			= $(addprefix $(OBJ_DIR), $(OBJ_FILES))

all: $(NAME)

bonus: $(NAME)

$(NAME): ECHO_COMPIL $(OBJ)
	@echo -n Compiling libft...
	@make -sC ./libft
	@echo Done.
	@echo -n Compiling executable $(NAME)...
	@$(CC) $(CFLAGS) $(OBJ) -lreadline -L.local/lib -L./libft -lft -o $(NAME)
	@echo Done.

ECHO_COMPIL:
	@echo Compiling minishell files...

$(OBJ_DIRS):
	@mkdir -p $@

$(OBJ): | $(OBJ_DIRS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADER)
	@$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

clean:
	@echo -n Making clean...
	@make clean -sC ./libft/
	@rm -rf $(OBJ_DIR)
	@echo Done.

fclean: clean
	@echo -n Making fclean...
	@make fclean -sC ./libft/
	@rm -f $(NAME)
	@echo Done.

re: fclean all

.PHONY : all clean fclean re
