/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_step2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 20:02:39 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/25 16:41:17 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_ambigous_redirection(t_pipe_command *cmd, int i, int len_of_file)
{
	char	*ambigous_redirect;

	print_message("Minishell: ", RED, 0);
	ambigous_redirect = ft_filedup(&cmd->cmd_content[i], len_of_file);
	print_message(ambigous_redirect, RED, 0);
	print_message(": Ambigous redirection\n", RED, 0);
	cmd->redirection_error = 2;
}

char	*generate_tmp_file_name(void)
{
	int		nbr;
	char	*nbr_str;
	char	*file_name;

	nbr = -2147483648;
	while (nbr < 2147483647)
	{
		nbr_str = ft_itoa(nbr, LOOP);
		if (!nbr_str)
			return (print_message(MALLOC_ERR_MSG, RED, 1), NULL);
		file_name = ft_strjoin("/tmp/", nbr_str, LOOP);
		if (file_name == NULL)
			return (print_message(MALLOC_ERR_MSG, RED, 1), NULL);
		if (access(file_name, F_OK) == -1)
			return (file_name);
		nbr++;
	}
	return (NULL);
}

void	*write_in_tmp_file(t_pipe_command *cmd)
{
	char	*tmp_file;

	tmp_file = generate_tmp_file_name();
	if (!tmp_file)
		return (errno_file_error("tmpfile", 0), NULL);
	cmd->fd_redirection[FD_IN]
		= open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (cmd->fd_redirection[FD_IN] == -1)
		return (errno_file_error(tmp_file, 0), NULL);
	write(cmd->fd_redirection[FD_IN], cmd->heredoc_str,
		ft_strlen(cmd->heredoc_str));
	close(cmd->fd_redirection[FD_IN]);
	cmd->fd_redirection[FD_IN] = open(tmp_file, O_RDONLY);
	if (cmd->fd_redirection[FD_IN] == -1)
		return (errno_file_error("tmpfile", 0), NULL);
	unlink(tmp_file);
	return ((void *)1);
}

void	*in_redirection_processing(t_pipe_command *cmd, int i, int len_of_file)
{
	if (!cmd->redirection_error)
	{
		if (access(cmd->infile, F_OK | W_OK) == -1)
		{
			cmd->redirection_error = 1;
			remove_file(len_of_file, &cmd->cmd_content[i]);
			return (errno_file_error(cmd->infile, 1), (void *)1);
		}
		cmd->fd_redirection[FD_IN] = open(cmd->infile, O_RDONLY);
		if (cmd->fd_redirection[FD_IN] == -1)
		{
			cmd->redirection_error = 1;
			remove_file(len_of_file, &cmd->cmd_content[i]);
			return (errno_file_error(cmd->infile, 1), (void *)1);
		}
	}
	return (NULL);
}

void	*in_redirection_parsing(t_pipe_command *cmd,
	char *operator, int i, char ***envp)
{
	int		len_of_file;

	if (cmd->fd_redirection[FD_IN] != 0)
		close(cmd->fd_redirection[FD_IN]);
	cmd->infile_operator = operator;
	while (cmd->cmd_content[i] == operator[0] || cmd->cmd_content[i] == ' ')
		ft_strcpy(&cmd->cmd_content[i], &cmd->cmd_content[i + 1]);
	len_of_file = file_len(&cmd->cmd_content[i]);
	if (ft_strncmp(operator, "<<", 2))
	{
		cmd->infile = ft_filedup(&cmd->cmd_content[i], len_of_file);
		redirection_var_expand(1, &cmd->infile, envp, "?\'\"_@#*-");
		if (!cmd->infile[0])
			print_ambigous_redirection(cmd, i, len_of_file);
		if (in_redirection_processing(cmd, i, len_of_file))
			return ((void *)1);
	}
	else if (!cmd->redirection_error && !ft_strstr(cmd->cmd_content, "<<")
		&& !ft_strchr(cmd->cmd_content, '<') && cmd->heredoc_str)
		if (!write_in_tmp_file(cmd))
			return (NULL);
	remove_file(len_of_file, &cmd->cmd_content[i]);
	return ((void *)1);
}
