/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_heredoc_loop.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 00:44:17 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/25 15:44:07 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*heredoc_loop(char *end_word, int var_expand, char ***envp)
{
	char	*input;
	char	*save;

	input = "";
	save = "";
	if (isquote_in(end_word))
		remove_quotes_str(&end_word);
	input = garbage_addptr(readline("> "), LOOP);
	if (check_eof_heredoc(input) || ft_strequ(input, end_word))
		return (save);
	redirection_var_expand(var_expand, &input, envp, "?$_@#*-");
	while (!ft_strequ(input, end_word))
	{
		save = ft_strjoin(save, input, LOOP);
		save = ft_strjoin(save, "\n", LOOP);
		if (!save)
			print_message("minishell: Allocation error.\n", RED, 1);
		input = garbage_addptr(readline("> "), LOOP);
		if (check_eof_heredoc(input))
			break ;
		if (ft_strequ(input, end_word))
			break ;
		redirection_var_expand(var_expand, &input, envp, "?$_@#*-");
	}
	return (save);
}

static char	*write_in_tmp_file(t_pipe_command *cmd, int i, char ***envp)
{
	int		len_of_file;
	char	*end_word;
	int		var_expand;
	char	*input;

	while (cmd->cmd_content[i] == ' ' || cmd->cmd_content[i] == '\t')
		i++;
	len_of_file = file_len(&cmd->cmd_content[i]);
	end_word = ft_filedup(&cmd->cmd_content[i], len_of_file);
	var_expand = 1;
	if (ft_strchr(end_word, '\'') || ft_strchr(end_word, '"'))
		var_expand = 0;
	input = heredoc_loop(end_word, var_expand, envp);
	if (g_status == 130)
		return (NULL);
	return (input);
}

uint8_t	find_heredoc(t_pipe_command *cmd, int file_nbr, char ***envp)
{
	int		i;
	char	*file_nbr_str;

	i = 0;
	file_nbr_str = ft_itoa(file_nbr, LOOP);
	if (!file_nbr_str)
		print_message("minishell: Allocation error.\n", RED, 1);
	while (cmd->cmd_content[i])
	{
		if (cmd->cmd_content[i] == '"')
			i += quote_len(&(cmd->cmd_content)[i], '"') + 1;
		else if (cmd->cmd_content[i] == '\'')
			i += quote_len(&(cmd->cmd_content)[i], '\'') + 1;
		else if (!ft_strncmp(&cmd->cmd_content[i], "<<", 2))
		{
			i += 2;
			cmd->heredoc_str = write_in_tmp_file(cmd, i, envp);
			if (cmd->heredoc_str == NULL || g_status == 130)
				return (1);
		}
		else if (cmd->cmd_content[i])
			i++;
	}
	return (0);
}

uint8_t	heredoc_management(t_list *list, char ***envp)
{
	t_list	*tmp;
	int		y;
	int		x;

	if (!list)
		return (0);
	y = 0;
	x = 0;
	while (list)
	{
		tmp = ((t_command *)list->content)->command_list;
		while (tmp)
		{
			if (find_heredoc((t_pipe_command *)tmp->content, x * 10 + y, envp))
				return (1);
			tmp = tmp->next;
			y++;
		}
		list = list->next;
		x++;
	}
	return (0);
}
