/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_step1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 20:02:39 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/25 15:48:13 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	file_len(char *s)
{
	int		len;

	len = 0;
	while (s[len] && s[len] != ' ' && s[len] != '|'
		&& s[len] != '&' && s[len] != '<' && s[len] != '>')
	{
		if (s[len] == '"')
		{
			len += quote_len(&s[len], '"') + 1;
		}
		else if (s[len] == '\'')
			len += quote_len(&s[len], '\'') + 1;
		else
			len++;
	}
	return (len);
}

char	*ft_filedup(char *s, int len_of_file)
{
	char	*dest;
	int		i;

	if (mem_alloc(sizeof(char) * (len_of_file + 1), (void **)&dest, LOOP))
		print_message("minishell: Allocation error.\n", RED, 1);
	i = 0;
	while (i < len_of_file)
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void	remove_file(int len_of_file, char *s)
{
	int	j;

	j = 0;
	while (j < len_of_file)
	{
		ft_strcpy(&s[0], &s[1]);
		j++;
	}
}

void	*out_redirection_processing(t_pipe_command *cmd, char *operator)
{
	if (!cmd->redirection_error)
	{
		if (!access(cmd->outfile, F_OK) && access(cmd->outfile, W_OK) == -1)
		{
			cmd->redirection_error = 1;
			return (errno_file_error(cmd->outfile, 1), (void *)1);
		}
		if (!ft_strncmp(operator, ">>", 2))
			cmd->fd_redirection[FD_OUT]
				= open(cmd->outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
		else if (!ft_strncmp(operator, ">", 1))
			cmd->fd_redirection[FD_OUT]
				= open(cmd->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (cmd->fd_redirection[FD_OUT] == -1)
		{
			cmd->redirection_error = 1;
			errno_file_error(cmd->outfile, 1);
		}
	}
	return (NULL);
}

void	*out_redirection_parsing(t_pipe_command *cmd,
	char *operator, int i, char ***envp)
{
	int		len_of_file;

	if (cmd->fd_redirection[FD_OUT] != 1)
		close(cmd->fd_redirection[FD_OUT]);
	cmd->outfile_operator = operator;
	while (cmd->cmd_content[i] == operator[0] || cmd->cmd_content[i] == ' ')
		ft_strcpy(&cmd->cmd_content[i], &cmd->cmd_content[i + 1]);
	len_of_file = file_len(&cmd->cmd_content[i]);
	cmd->outfile = ft_filedup(&cmd->cmd_content[i], len_of_file);
	redirection_var_expand(1, &cmd->outfile, envp, "?\'\"_@#*-");
	if (!cmd->outfile[0])
		print_ambigous_redirection(cmd, i, len_of_file);
	remove_file(len_of_file, &cmd->cmd_content[i]);
	out_redirection_processing(cmd, operator);
	return ((void *)1);
}
