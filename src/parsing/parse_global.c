/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_global.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 13:58:56 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/26 17:01:54 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*global_parsing(char *input, int save_in)
{
	t_list	*list;

	remove_comments(&input);
	list = parse_step1(input);
	if (!list)
		return (NULL);
	if (!single_pipe_parsing(&list, save_in))
		return (NULL);
	return (list);
}

void	command_parsing(t_list *command_list, char **envp[])
{
	cmd_redirection_management(command_list, envp);
	variable_expansion(command_list, envp);
}
