/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_step2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 18:15:02 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/25 15:33:29 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	iter_trim(t_list **lst)
{
	t_list	*tmp_lst;
	char	*tmp_string;

	tmp_lst = *lst;
	while (tmp_lst)
	{
		tmp_string = ft_strtrim
			(((t_pipe_command *)tmp_lst->content)->cmd_content, " ", LOOP);
		mem_remove(((t_pipe_command *)tmp_lst->content)->cmd_content, LOOP);
		((t_pipe_command *)tmp_lst->content)->cmd_content = tmp_string;
		tmp_lst = tmp_lst->next;
	}
}

t_pipe_command	*set_pipe_cmd_node(char *pipe_cmd, int save_in)
{
	t_pipe_command	*pipe_struct;

	if (mem_alloc(sizeof(t_pipe_command), (void **) &pipe_struct, LOOP))
		print_message("minishell: Allocation error.\n", RED, 1);
	pipe_struct->save_in = save_in;
	pipe_struct->infile = NULL;
	pipe_struct->outfile = NULL;
	pipe_struct->heredoc_str = NULL;
	pipe_struct->fd_redirection[FD_IN] = 0;
	pipe_struct->fd_redirection[FD_OUT] = 1;
	pipe_struct->fd_pipe[FD_IN] = 0;
	pipe_struct->fd_pipe[FD_OUT] = 0;
	pipe_struct->exec_args = NULL;
	pipe_struct->cmd_content = pipe_cmd;
	pipe_struct->redirection_error = 0;
	return (pipe_struct);
}

t_list	*set_pipe_commands_list(char *command, int save_in)
{
	int				i;
	char			**pipe_cmds;
	t_list			*pipe_cmd_list;
	t_list			*tmp;
	t_pipe_command	*cmd_struct;

	i = 0;
	pipe_cmd_list = NULL;
	pipe_cmds = ft_split_noquote(command, "|", LOOP);
	if (!pipe_cmds)
		print_message("minishell: Allocation error.\n", RED, 1);
	while (pipe_cmds[i] != NULL)
	{
		cmd_struct = set_pipe_cmd_node(pipe_cmds[i], save_in);
		tmp = ft_lstnew(cmd_struct, LOOP);
		if (!tmp)
			print_message("minishell: Allocation error.\n", RED, 1);
		ft_lstadd_back(&pipe_cmd_list, tmp);
		i++;
	}
	mem_remove(pipe_cmds, LOOP);
	return (pipe_cmd_list);
}

void	*single_pipe_parsing(t_list **list_first_parse, int save_in)
{
	t_list	*tmp;

	tmp = *list_first_parse;
	while (tmp)
	{
		((t_command *)tmp->content)->command_list = set_pipe_commands_list
			(((t_command *)tmp->content)->command, save_in);
		if (!((t_command *)tmp->content)->command_list)
			return (NULL);
		iter_trim(&((t_command *)tmp->content)->command_list);
		tmp = tmp->next;
	}
	return ((void *) 1);
}
