/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_not_handled.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:02:46 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/25 15:27:15 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	quote_len(char *sub_input, char quote)
{
	int	i;

	i = 1;
	while (sub_input[i] && sub_input[i] != quote)
		i++;
	return (i);
}

static int	quote_deplacement(int *i, int quote_nbr, char *input)
{
	if (input[*i] == '"')
	{
		quote_nbr += 1;
		*i += quote_len(&input[*i], '"');
		if (input[*i] == '"')
			quote_nbr++;
	}
	else if (input[*i] == '\'')
	{
		quote_nbr += 1;
		*i += quote_len(&input[*i], '\'');
		if (input[*i] == '\'')
			quote_nbr += 1;
	}
	return (quote_nbr);
}

uint8_t	open_quotes(char *input)
{
	int	i;
	int	quote_nbr;

	i = 0;
	quote_nbr = 0;
	while (input[i])
	{
		quote_nbr = quote_deplacement(&i, quote_nbr, input);
		if (input[i])
			i++;
	}
	if (quote_nbr % 2 == 0)
		return (0);
	return (1);
}

static uint8_t	check_not_interpreted(char *sub_input, int i)
{
	if (sub_input[i] == ';')
		return (1);
	else if (sub_input[i] == '[')
		return (1);
	else if (sub_input[i] == ']')
		return (1);
	else if (sub_input[i] == '\\')
		return (1);
	else if (sub_input[i] == '!')
		return (1);
	else if (sub_input[i] == '&')
		return (1);
	else if (sub_input[i] == '`')
		return (1);
	else if (!ft_strncmp(&sub_input[i], ">|", 2))
		return (1);
	else if (!ft_strncmp(&sub_input[i], "<>", 2))
		return (1);
	else if (!ft_strncmp(&sub_input[i], "<<<", 3))
		return (1);
	return (0);
}

uint8_t	not_interpreted_characters(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '"')
			i += quote_len(&input[i], '"');
		if (input[i] == '\'')
			i += quote_len(&input[i], '\'');
		else if (!strncmp(&input[i], "&&", 2))
			i += 2;
		else if (check_not_interpreted(input, i))
			return (1);
		if (input[i])
			i++;
	}
	return (0);
}
