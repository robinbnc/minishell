/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_step3_variable.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 00:38:24 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/25 15:37:51 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static uint8_t	previous_token_ismeta(char *command, int i)
{
	while (i > 0 && command[i] != ' ' && command[i] != '\t'
		&& command[i] != '>' && command[i] != '<')
		i--;
	while (i > 0 && (command[i] == ' ' || command[i] == '\t'))
		i--;
	if (i >= 0 && ft_ischarset(command[i], "<>", NULL))
	{
		if (command[i] == '>')
		{
			if (i > 0 && command[i - 1] == '>')
				return (2);
			return (1);
		}
		if (command[i] == '<')
		{
			if (i > 0 && command[i - 1] == '<')
			{
				return (4);
			}
			return (3);
		}
	}
	return (0);
}

char	*get_to_insert(char *s, int pos, int size, char **envp[])
{
	char	*to_find;
	char	*to_insert;

	if (mem_alloc(size, (void **)&to_find, LOOP))
		print_message("Minishell: Allocation error.\n", RED, 1);
	ft_strlcpy(to_find, s + pos + 1, size);
	to_insert = get_env(to_find, envp);
	mem_remove(to_find, LOOP);
	if (previous_token_ismeta(s, pos) == 4)
		return (NULL);
	else if (previous_token_ismeta(s, pos) != 0)
		to_insert = ft_stradd_quotes(to_insert, LOOP);
	if (to_insert == NULL)
		print_message("Minishell: Allocation error\n", RED, 1);
	return (to_insert);
}

static int	insert(t_list *command_list, int start, char **envp[])
{
	int		stop;
	char	*to_insert;
	char	*command;

	to_insert = NULL;
	command = ((t_pipe_command *)command_list->content)->cmd_content;
	stop = stop_len(command, start);
	if (command[start + 1] == '?')
	{
		to_insert = ft_itoa(g_status, LOOP);
		if (!to_insert)
			print_message("minishell: Allocation error.\n", RED, 1);
	}
	else if (ft_ischarset(command[start + 1], "\'\"_$@#*-", ft_isalnum))
	{
		to_insert = get_to_insert(command, start, stop, envp);
		if (!to_insert)
			return (0);
	}
	if (ft_strinsert(&command, to_insert, start, stop))
		print_message("Minishell: Allocation error.\n", RED, 1);
	((t_pipe_command *)command_list->content)->cmd_content = command;
	return ((int)ft_strlen(to_insert));
}

static void	jump_quotes(char *cmd, int *double_quote, int *i)
{
	if (cmd[*i] && !(*double_quote) && cmd[*i] == '\'')
	{
		*i += 1;
		while (cmd[*i] && cmd[*i] != '\'')
			*i += 1;
	}
	if (cmd[*i] && !(*double_quote) && cmd[*i] == '"')
		*double_quote = 1;
	else if (cmd[*i] && *double_quote && cmd[*i] == '"')
		*double_quote = 0;
}

void	variable_expansion(t_list *command_list, char **envp[])
{
	int		i;
	int		double_quote;
	char	*command;

	while (command_list)
	{
		i = 0;
		double_quote = 0;
		command = ((t_pipe_command *)command_list->content)->cmd_content;
		while (command[i])
		{
			jump_quotes(command, &double_quote, &i);
			if (command[i] == '$'
				&& (ft_ischarset(command[i + 1], "?_$@#*-", ft_isalnum)
					|| (!double_quote
						&& ft_ischarset(command[i + 1], "\'\"", NULL))))
				i += insert(command_list, i, envp) - 1;
			command = ((t_pipe_command *)command_list->content)->cmd_content;
			i++;
		}
		command_list = command_list->next;
	}
}
