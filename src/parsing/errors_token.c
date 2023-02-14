/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 19:40:20 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/24 13:56:00 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*unexpected_next(char *str, int index)
{
	while (str[index] == ' ')
		index++;
	if (!ft_strncmp(&str[index], "||", 2))
		return ("||");
	else if (!ft_strncmp(&str[index], "&&", 2))
		return ("&&");
	else if (str[index] == '|')
		return ("|");
	else if (!ft_strncmp(&str[index], ">>", 2))
		return (">>");
	else if (!ft_strncmp(&str[index], "<<", 2))
		return ("<<");
	else if (str[index] == '>')
		return (">");
	else if (str[index] == '<')
		return ("<");
	else if (str[index] == '\0')
		return ("newline");
	return (NULL);
}

uint8_t	unexpected_prev(char *str, int index)
{
	if (index > 0)
		index--;
	while (index > 0 && str[index] == ' ')
		index--;
	if (str[index] == '|' || str[index] == '&'
		|| str[index] == '>' || str[index] == '<' || str[index] == ' ')
		return (1);
	return (0);
}

char	*redirection_error(char *input, int *i)
{
	if (!ft_strncmp(&input[*i], ">>", 2))
	{
		if (unexpected_next(input, *i + 2))
			return (unexpected_next(input, *i + 2));
		*i += 1;
	}
	else if (!ft_strncmp(&input[*i], "<<", 2))
	{
		if (unexpected_next(input, *i + 2))
			return (unexpected_next(input, *i + 2));
		*i += 1;
	}
	else if (input[*i] == '>' && unexpected_next(input, *i + 1))
	{
		return (unexpected_next(input, *i + 1));
	}
	else if (input[*i] == '<' && unexpected_next(input, *i + 1))
		return (unexpected_next(input, *i + 1));
	return (NULL);
}

char	*charset_token_error(char *input)
{
	int		i;
	char	*ret;

	i = 0;
	while (input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
			i += quote_len(&input[i], input[i]);
		else if (!ft_strncmp(&input[i], "||", 2))
		{
			if (unexpected_prev(input, i))
				return ("||");
			i += 1;
		}
		else if (!ft_strncmp(&input[i], "&&", 2) && unexpected_prev(input, i))
			return ("&&");
		else if (input[i] == '|' && unexpected_prev(input, i))
			return ("|");
		ret = redirection_error(input, &i);
		if (ret)
			return (ret);
		else if (input[i])
		i++;
	}
	return (NULL);
}

uint8_t	near_unexpected_token_error(char **input, char **shell_prompt,
	char **envp[])
{
	char	*unexpected_token;

	unexpected_token = charset_token_error(*input);
	if (not_interpreted_characters(*input))
		return (*input = "", print_first_check_error(WRONG_CHAR_ERR_MSG,
				NULL, envp, shell_prompt));
	else if (unexpected_token)
		return (*input = "", print_first_check_error(NEAR_TOKEN_ERR_MSG,
				unexpected_token, envp, shell_prompt));
	else if (open_quotes(*input))
		return (*input = "", print_first_check_error(QUOTES_ERR_MSG,
				NULL, envp, shell_prompt));
	else if (parenthesis_checker(input, envp, shell_prompt))
		return (*input = "", 1);
	else if (pipe_is_open(*input))
		return (*input = "", print_first_check_error(OPEN_PIPE_ERR_MSG,
				NULL, envp, shell_prompt));
	return (0);
}
