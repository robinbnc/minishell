/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_heredoc_variables.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 00:44:17 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/25 15:46:09 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	insert_var_str(char **str, int start, char ***envp, char *charset)
{
	int		stop;
	char	*to_insert;
	char	*command;

	to_insert = NULL;
	command = *str;
	stop = stop_len(command, start);
	if (command[start + 1] == '?')
	{
		to_insert = ft_itoa(g_status, LOOP);
		if (!to_insert)
			print_message("Minishell: Allocation error.\n", RED, 1);
	}
	else if (ft_ischarset(command[start + 1], charset, ft_isalnum))
	{
		to_insert = get_to_insert(command, start, stop, envp);
		if (!to_insert)
			return (0);
	}
	if (ft_strinsert(&command, to_insert, start, stop))
		print_message("Minshell: Allocation error.\n", RED, 1);
	*str = command;
	return ((int)ft_strlen(to_insert));
}

void	redirection_var_expand(int var_expand, char **input,
	char ***envp, char *charset)
{
	int		i;
	int		double_quote;

	i = 0;
	if (var_expand == 1)
	{
		double_quote = 0;
		while ((*input)[i])
		{
			if ((*input)[i] == '$' && ft_ischarset((*input)[i + 1],
				charset, ft_isalnum))
				i += insert_var_str(input, i, envp, charset);
			jump_quotes(*input, &double_quote, &i);
			if ((*input)[i])
				i++;
		}
	}
}
