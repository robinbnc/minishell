/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_parenthesis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 20:14:31 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/25 15:28:38 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static uint8_t	is_meta(char *sub_input, int i, int direction)
{
	if (direction == PREVIOUS)
	{
		while (--i >= 0)
		{
			if (sub_input[i] == ' ' || sub_input[i] == '(')
				continue ;
			else
				return (check_meta(sub_input, i, i > 0, i - 1));
		}
		if (i < 0)
			return (1);
	}
	else if (direction == NEXT)
	{
		while (sub_input[++i])
		{
			if (sub_input[i] == ' ' || sub_input[i] == ')')
				continue ;
			else
				return (check_meta(sub_input, i, 1, i + 1));
		}
		if (!sub_input[i])
			return (1);
	}
	return (1);
}

static uint8_t	open_parent(char **input, char **envp[], char **shell_prompt)
{
	int	i;
	int	ret;
	int	left_nbr;
	int	right_nbr;

	i = 0;
	left_nbr = 0;
	right_nbr = 0;
	while ((*input)[i])
	{
		ret = check_parent(&i, input, &left_nbr, &right_nbr);
		if (ret == 1 || ret == 3)
			return (print_first_check_error(NEAR_TOKEN_ERR_MSG, ")",
					envp, shell_prompt));
		else if (ret == 2)
			return (print_first_check_error(PARENTHESIS_ERR_MSG, NULL,
					envp, shell_prompt));
		if ((*input)[i])
			i++;
	}
	if (left_nbr == right_nbr)
		return (0);
	return (print_first_check_error(PARENTHESIS_ERR_MSG, NULL,
			envp, shell_prompt));
}

static char	*get_token(char *input, int i)
{
	int		size;
	int		ret;
	char	*token;

	size = 0;
	while (input[++i] && input[i] == ' ')
		;
	if (input[i] == '\0')
		return ("newline");
	ret = 0;
	while (input[i] && !ft_ischarset(input[i], ") ", NULL))
	{
		if (input[i] == '\"' || input[i] == '\'')
		{
			ret += quotes_len(input, i) - 1;
			size += quotes_len(input, i) - 1;
			i += quotes_len(input, i) - 1;
		}	
		size++;
		ret++;
		i++;
	}
	if (mem_alloc(size + 1, (void **)&token, LOOP))
		print_message("minishell: Allocation error\n", RED, 1);
	return (ft_strlcpy(token, &input[i - ret], size + 1), token);
}

uint8_t	parenthesis_checker(char **input, char **envp[], char **shell_prompt)
{
	int	i;

	i = 0;
	if (open_parent(input, envp, shell_prompt))
		return (1);
	while ((*input)[i])
	{
		i += quotes_len(*input, i);
		if ((*input)[i] == '(' && !is_meta(*input, i, PREVIOUS))
			return (print_first_check_error(NEAR_TOKEN_ERR_MSG,
					get_token(*input, i), envp, shell_prompt));
		else if ((*input)[i] == ')' && !is_meta(*input, i, NEXT))
			return (print_first_check_error(NEAR_TOKEN_ERR_MSG,
					get_token(*input, i), envp, shell_prompt));
		if ((*input)[i])
			i++;
	}
	return (0);
}
