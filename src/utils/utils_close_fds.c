/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_close_fds.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 23:34:05 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/25 16:17:44 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_all_fds(t_pipe_command *command, t_pipe_command *prev,
			t_list *command_list)
{
	if (command->fd_redirection[FD_IN] != 0)
		close(command->fd_redirection[FD_IN]);
	if (command->fd_redirection[FD_OUT] != 1)
		close(command->fd_redirection[FD_OUT]);
	if (command->pid == 0 && command_list->next != NULL)
	{
		close(command->fd_pipe[FD_OUT]);
		close(command->fd_pipe[FD_IN]);
	}
	else if (prev != NULL)
		close(prev->fd_pipe[FD_IN]);
	if (command_list->next != NULL)
		close(command->fd_pipe[FD_OUT]);
}

void	close_all_fds_cmd_list(t_list *command_list, uint8_t ignore_fd_pipe)
{
	t_pipe_command	*el;

	while (command_list)
	{
		el = ((t_pipe_command *)command_list->content);
		if (el->fd_redirection[FD_IN] != 0)
			close(el->fd_redirection[FD_IN]);
		if (el->fd_redirection[FD_OUT] != 1)
			close(el->fd_redirection[FD_OUT]);
		if (ignore_fd_pipe && el->fd_pipe[FD_IN] != 0)
			close(el->fd_pipe[FD_IN]);
		if (ignore_fd_pipe && el->fd_pipe[FD_OUT] != 0)
			close(el->fd_pipe[FD_OUT]);
		command_list = command_list->next;
	}
}
