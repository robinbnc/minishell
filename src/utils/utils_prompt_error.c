/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_prompt_error.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:51:21 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/25 16:55:02 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint8_t	pipe_is_open(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = ft_strlen(str) - 1;
	while (str[i] == ' ' && i >= 0)
		i--;
	if (str[i] == '|' || (str[i] == '&' && (i > 0 && str[i - 1] == '&')))
		return (1);
	return (0);
}

int8_t	input_error(char **input, char **shell_prompt,
	char **envp[], int save_in)
{
	if (!*input)
	{
		close(save_in);
		eof_exit();
	}
	*input = ft_strtrim(*input, " ", LOOP);
	if (!*input)
		print_message("Minishell: Allocation error.\n", RED, 1);
	if (ft_strequ(*input, ""))
		return (-1);
	if (near_unexpected_token_error(input, shell_prompt, envp))
		return (-1);
	return (0);
}
