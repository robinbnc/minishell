/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 00:44:17 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/25 16:54:09 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint8_t	check_eof_heredoc(char *input)
{
	if (!input)
	{
		if (g_status == 130)
			return (1);
		else
		{
			ft_putendl_fd(EOF_HEREDOC, 2);
			return (1);
		}
	}
	return (0);
}

char	*ft_strndup(const char *s, int garbage, size_t len)
{
	char	*sdup;

	if (mem_alloc(len * sizeof(char), (void **)&sdup, garbage))
		print_message("Minishell: Allocation error.\n", RED, 1);
	ft_strlcpy(sdup, s, len);
	return (sdup);
}

uint8_t	control_op_present(t_list *tmp, char *input, int *i)
{
	if (input[*i] == '|' && input[*i + 1] == '|')
	{
		((t_command *)tmp->content)->control_op = "||";
		*i += 2;
		return (1);
	}
	if (input[*i] == '&' && input[*i + 1] == '&')
	{
		((t_command *)tmp->content)->control_op = "&&";
		*i += 2;
		return (1);
	}
	return (0);
}
