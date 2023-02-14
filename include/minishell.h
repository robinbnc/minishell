/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 17:59:05 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/26 17:02:15 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <setjmp.h>
# include <errno.h>
# include <error.h>
# include <getopt.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdint.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include "../libft/include/libft.h"

/*
**	Colors definition
*/

# define RED     "\x1b[31;1m"
# define GREEN   "\x1b[32m"
# define YELLOW  "\x1b[33m"
# define BLUE    "\x1b[34m"
# define MAGENTA "\x1b[35m"
# define CYAN    "\x1b[36m"
# define RESET   "\x1b[0m"

/*
**	Garbages
*/

# define MAIN 0
# define LOOP 1
# define FDS  2

/*
**	Defines
*/

# define DO_NOT_CLOSE_PROG 0
# define PREVIOUS 1
# define NEXT 2

/*
**	Structures
*/

typedef struct s_pipe_command
{
	int		save_in;
	char	*heredoc_str;
	int		redirection_error;
	char	*infile_operator;
	char	*outfile_operator;
	char	*infile;
	char	*outfile;
	int		fd_redirection[2];
	int		fd_pipe[2];
	char	*cmd_content;
	char	**exec_args;
	pid_t	pid;
}	t_pipe_command;

typedef struct s_command
{
	char	*control_op;
	char	*command;
	t_list	*command_list;
	int		nb_parenthesis;
}	t_command;

typedef struct s_export
{
	char	*var;
	char	*equal;
	char	*value;
}				t_export;
/*
**	Global variables
*/

extern int	g_status;

/*
**	Terminal / print functions
*/

void	print_start(void);
char	*create_prompt(char **envp[]);
void	prompt_loop(char **envp[], char *input, char *shell_prompt);
uint8_t	pipe_is_open(char *str);
void	print_message(char *msg, char *color, int exit_status);
void	errno_file_error(char *file, int exit_status);

/*
**	Parsing
*/

t_list	*global_parsing(char *input, int save_in);
void	remove_comments(char **input);
t_list	*parse_step1(char *input);
void	*single_pipe_parsing(t_list **list_first_parse, int save_in);
char	**ft_split_pipe(const char *s, char *sep);//test

void	command_parsing(t_list *command_list, char **envp[]);
void	variable_expansion(t_list *command_list, char **envp[]);
void	split_args(t_list *command_list);
void	remove_quotes_list(t_list *command_list);
void	wildcard(t_list *command_list);
uint8_t	isquote_in(char *str);
uint8_t	control_op_present(t_list *tmp, char *input, int *i);

// char	*get_to_insert(char *s, int pos, int size);
int		stop_len(char *s, int start);

void	remove_quotes_str(char **str);

/*
**	Signals
*/

void	sig_handler(int sigcode);
void	sig_void(int sigcode);

/*
**	Execution
*/

uint8_t	exec_main(t_list *cmd_list, char **envp[]);
void	exec_bin(t_pipe_command *command, char **envp[]);
int		exec_builtin(t_pipe_command *command, char **envp[], int exit, int fd);
uint8_t	forking(t_list *command_list, char **envp[]);
void	wait_children(t_list *command_list);
char	*get_path(char **exec_args, char **envp[]);
void	child_sig_handler(int sigcode);

/*
**	Bultins
*/

int		builtin_exit(char **exec_args, int exit);
int		is_builtin(char *exec_args);
int		builtin_echo(char **exec_args, int exit, int fd);
int		builtin_pwd(char **exec_args, int exit, int fd, char **envp[]);
int		builtin_env(char **exec_args, char **envp[], int exit, int fd);
int		builtin_cd(char **exec_args, char **envp[], int exit);
int		builtin_export(char **exec_args, char **envp[],
			int fd, t_list **export_var);
void	export_var_exist(t_list **export_var, char *var_name);
int		builtin_unset(char **exec_args, char **envp[],
			int exit, t_list **export_var);
char	*env_variable_exist(char *envp[], char *param, size_t var_name_len);
int		close_save_fd(int fd);
t_list	*export_init_env(char *envp[], t_list **export_list);
void	print_strs_fd(t_list **export_var, int fd);
char	*ft_strndup(const char *s, int garbage, size_t len);
t_list	**add_el_to_export_list(t_list **export_list, char *export_el);
void	lst_bubblesort(t_list **list_to_sort);

/*
**	Exit
*/

void	free_and_exit(int exit_code);
void	child_error_exit(int status_error, char *file);
void	eof_exit(void);

/*
**	Utils
*/

char	*get_env(char *var, char **envp[]);
void	set_env(char *var, char *content, char **envp[]);
char	*path_troncate(char *s, char *to_troncate);
int		print_dirpath_err(char *path);
void	handle_shlvl(int nb, char **envp[]);
void	close_all_fds(t_pipe_command *command, t_pipe_command *prev,
			t_list *command_list);
void	close_all_fds_cmd_list(t_list *command_list, uint8_t ignore_fd_pipe);
uint8_t	is_star(char *file, char *arg, int arg_i, int file_i);
uint8_t	check_meta(char *sub_input, int i, int check, int j);
uint8_t	check_parent(int *i, char **input, int *left_nbr,
			int *right_nbr);

uint8_t	pipe_is_open(char *str);
int8_t	input_error(char **input, char **shell_prompt,
			char **envp[], int save_in);
/*
**	Error management
*/

# define PATH_CMD_ERR 127
# define PATH_FILE_ERR 127
# define RIGHT_FILE_ERR 126
# define EXEC_ERR 1
# define MALLOC_ERR 2
# define FD_ERR 1

# define ARGS_ERR_MSG "Error : Invalid number of arguments.\n"
# define QUOTES_ERR_MSG "Minishell: open quotes are not interpreted."
# define PARENTHESIS_ERR_MSG "Minishell: open parenthesis are not interpreted."
# define OPEN_PIPE_ERR_MSG "Minishell: open pipe are not interpreted."
# define WRONG_CHAR_ERR_MSG "Minishell: uninterpreted token \
present outside quotes."
# define NEAR_TOKEN_ERR_MSG "Minishell: syntax error near unexpected token"
# define UNEXPECTED_EOF "Minishell: syntax error: unexpected end of file.\n"
# define UNAVAILABLE_ENV "Minishell: Environment unavailable.\n"
# define EOF_HEREDOC "Minishell:\
 warning: here-document at line 1 delimited by end-of-file"
# define MALLOC_ERR_MSG "Minishell: Allocation error\n"
# define PWD_ERR_MSG "pwd: error retrieving current directory: getcwd: \
cannot access parent directories: No such file or directory"
# define DOT_ERR_MSG ": filename argument required\n\
.: usage: . filename [arguments]"

uint8_t	near_unexpected_token_error(char **input,
			char **shell_prompt, char **envp[]);
uint8_t	open_quotes(char *input);
uint8_t	parenthesis_checker(char **input, char **envp[], char **shell_prompt);
uint8_t	not_interpreted_characters(char *input);
uint8_t	print_first_check_error(char *msg, char *token,
			char **envp[], char **shell_prompt);
int		quote_len(char *sub_input, char quote);

/*
**	redirections
*/

# define FD_IN 0
# define FD_OUT 1

int		cmd_redirection_management(t_list *list, char ***envp);
int		file_len(char *s);
char	*ft_filedup(char *s, int len_of_file);
void	remove_file(int len_of_file, char *s);
void	*out_redirection_parsing(t_pipe_command *cmd,
			char *operator, int i, char ***envp);
void	*in_redirection_parsing(t_pipe_command *cmd,
			char *operator, int i, char ***envp);
uint8_t	heredoc_management(t_list *list, char ***envp);
void	close_heredoc_fds(t_list *list);
char	*get_to_insert(char *s, int pos, int size, char **envp[]);
int		stop_len(char *s, int start);
void	remove_quotes_str(char **str);
uint8_t	check_eof_heredoc(char *input);
void	redirection_var_expand(int var_expand,
			char **input, char ***envp, char *charset);
int		ambigous_redirect(char *file);
void	print_ambigous_redirection(t_pipe_command *cmd, int i, int len_of_file);

#endif
