/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 20:29:44 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/25 18:58:57 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint8_t	input_first_read(char **input, char **shell_prompt,
	char **envp[], int save_in)
{
	int	save_status;

	save_status = g_status;
	g_status = -256;
	*input = garbage_addptr(readline(*shell_prompt), LOOP);
	if (g_status == 130)
		return (2);
	else if (g_status < -256)
		g_status = 130;
	else
		g_status = save_status;
	if (input_error(input, shell_prompt, envp, save_in))
		return (1);
	return (0);
}

int	first_read_processing(int *save_in, char **input,
	char **shell_prompt, char **envp[])
{
	int		ret_first_read;

	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	signal(SIGTSTP, sig_handler);
	*save_in = dup(STDIN_FILENO);
	ret_first_read = input_first_read(input, shell_prompt, envp, *save_in);
	if (ret_first_read == 1)
	{
		close(*save_in);
		return (2);
	}
	if (ret_first_read == 2)
	{
		dup2(*save_in, STDIN_FILENO);
		close(*save_in);
		return (1);
	}
	return (0);
}

uint8_t	parsing_and_exec_processing(int save_in, char **input, char **envp[])
{
	t_list	*cmd_list;

	cmd_list = global_parsing(*input, save_in);
	if (!ft_strequ(*input, ""))
		add_history(*input);
	if (!cmd_list)
		return (1);
	if (!exec_main(cmd_list, envp))
		return (0);
	return (1);
}

void	prompt_loop(char **envp[], char *input, char *shell_prompt)
{
	int		save_in;
	int		new_line;
	int		ret_first_read;

	new_line = 0;
	while (1)
	{
		if (new_line)
			ft_putendl_fd("", 2);
		new_line = 1;
		ret_first_read = first_read_processing(&save_in, &input,
				&shell_prompt, envp);
		if (ret_first_read)
		{
			if (ret_first_read == 2)
				new_line = 0;
			continue ;
		}
		new_line = parsing_and_exec_processing(save_in, &input, envp);
		dup2(save_in, STDIN_FILENO);
		close(save_in);
		del_garbage(LOOP);
		input = "";
		shell_prompt = create_prompt(envp);
	}
}
