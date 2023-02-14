/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_step3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 20:02:39 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/25 16:42:29 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ambigous_redirect(char *file)
{
	if (file[0] == '$')
	{
		print_message("Minishell: ", RED, 0);
		print_message(file, RED, 0);
		print_message(": ambiguous redirect\n", RED, 0);
		return (1);
	}
	return (0);
}

uint8_t	pipe_redirection_management(t_pipe_command *cmd, char ***envp)
{
	int		i;
	void	*ret;

	i = 0;
	ret = (void *)1;
	while (cmd->cmd_content[i])
	{
		if (cmd->cmd_content[i] == '"')
			i += quote_len(&(cmd->cmd_content)[i], '"') + 1;
		else if (cmd->cmd_content[i] == '\'')
			i += quote_len(&(cmd->cmd_content)[i], '\'') + 1;
		else if (!ft_strncmp(&cmd->cmd_content[i], ">>", 2))
			ret = out_redirection_parsing(cmd, ">>", i, envp);
		else if (!ft_strncmp(&cmd->cmd_content[i], "<<", 2))
			ret = in_redirection_parsing(cmd, "<<", i, envp);
		else if (cmd->cmd_content[i] == '>')
			ret = out_redirection_parsing(cmd, ">", i, envp);
		else if (cmd->cmd_content[i] == '<')
			ret = in_redirection_parsing(cmd, "<", i, envp);
		else if (cmd->cmd_content[i])
			i++;
		if (ret == NULL)
			return (1);
	}
	return (0);
}

int	cmd_redirection_management(t_list *list, char ***envp)
{
	t_list	*tmp;

	if (!list)
		return (0);
	tmp = list;
	while (tmp)
	{
		if (pipe_redirection_management((t_pipe_command *)tmp->content, envp))
			return (1);
		((t_pipe_command *)tmp->content)->cmd_content
			= ft_strtrim(((t_pipe_command *)tmp->content)->cmd_content,
				" ", LOOP);
		if (!((t_pipe_command *)tmp->content)->cmd_content)
			print_message("minishell: Allocation error.\n", RED, 1);
		tmp = tmp->next;
	}
	return (0);
}
