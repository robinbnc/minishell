/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_step4_splitargs.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 00:39:47 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/25 15:38:49 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	split_args(t_list *command_list)
{
	char	*command;

	while (command_list)
	{
		command = ((t_pipe_command *)command_list->content)->cmd_content;
		((t_pipe_command *)command_list->content)->exec_args
			= ft_split_noquote(command, " ", LOOP);
		if (!((t_pipe_command *)command_list->content)->exec_args)
			print_message("minishell: Allocation error.\n", RED, 1);
		command_list = command_list->next;
	}
}
