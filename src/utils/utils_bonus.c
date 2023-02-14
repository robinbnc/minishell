/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 15:19:17 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/25 16:17:40 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint8_t	is_star(char *file, char *arg, int arg_i, int file_i)
{
	if (file[file_i] && (!ft_strnequ(&file[file_i], &arg[arg_i],
				ft_strlento(&arg[arg_i], '*'))
			|| (!ft_strnequ(&file[file_i], &arg[arg_i], ft_strlento
					(&arg[arg_i], '*') + 1) && !arg[arg_i + 1])))
		return (1);
	return (0);
}

static int	quotes_len(char *input, int i)
{
	int	save;

	save = i;
	if (input[i] == '\"')
		i += quote_len(&input[i], '\"') + 1;
	else if (input[i] == '\'')
		i += quote_len(&input[i], '\'') + 1;
	return (i - save);
}

uint8_t	check_meta(char *sub_input, int i, int check, int j)
{
	if (check && sub_input[i] == '|' && sub_input[j] == '|')
		return (1);
	else if (sub_input[i] == '&')
		return (1);
	else
		return (0);
}

uint8_t	check_parent(int *i, char **input, int *left_nbr,
		int *right_nbr)
{
	int	j;

	*i += quotes_len(*input, *i);
	if ((*input)[*i] == '(')
	{
		(*left_nbr)++;
		if ((*input)[*i + 1] == ')')
			return (1);
		j = *i;
		while ((*input)[++j] && (*input)[j] != ')')
		{
			if ((*input)[j] != ' ')
				break ;
		}
		if (!(*input)[j])
			return (2);
		if ((*input)[j] == ')')
			return (3);
	}
	else if ((*input)[*i] == ')')
		(*right_nbr)++;
	return (0);
}

int	stop_len(char *s, int start)
{
	int	end;

	end = 1;
	if (ft_ischarset(s[start + end], "?$@#*-", ft_isdigit))
		end++;
	else if (!ft_ischarset(s[start + end], "\'\"", NULL))
	{
		while (s[start + end] && ft_ischarset(s[start + end], "_", ft_isalnum))
			end++;
	}
	return (end);
}
