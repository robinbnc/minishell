/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:06:44 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/25 16:37:50 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_start(void)
{
	ft_putstr_fd("\n            $$\\         $$\\", 2);
	ft_putendl_fd("        $$\\             $$\\$$\\", 2);
	ft_putstr_fd("            \\_|        ", 2);
	ft_putendl_fd(" \\_|        $$|             $$|$$|", 2);
	ft_putstr_fd("$$$$$\\$$$\\  $$\\ $$$$$$\\ $$\\", 2);
	ft_putendl_fd(" $$$$$$\\$$$$$$\\  $$$$$\\ $$|$$|", 2);
	ft_putstr_fd("$$ _$$ _$$\\ $$ |$$ __$$\\$$|", 2);
	ft_putendl_fd("$$  ___|$$ __$$\\$$ __$$\\$$|$$|", 2);
	ft_putstr_fd("$$/ $$/ $$ |$$ |$$|  $$|$$|", 2);
	ft_putendl_fd("\\$$$$$\\ $$|  $$|$$$$$$$|$$|$$|", 2);
	ft_putstr_fd("$$| $$| $$ |$$ |$$|  $$|$$|", 2);
	ft_putendl_fd(" \\___$$\\$$|  $$|$$  ___|$$|$$|", 2);
	ft_putstr_fd("$$| $$| $$ |$$ |$$|  $$|$$|", 2);
	ft_putendl_fd("$$$$$$ |$$|  $$|\\$$$$$$\\$$|$$|", 2);
	ft_putstr_fd("\\_| \\_| \\__|\\__|\\_|  \\_|\\_|", 2);
	ft_putendl_fd("\\_____/ \\_|  \\_| \\_____|\\_|\\_|\n", 2);
}

void	print_message(char *msg, char *color, int exit_status)
{
	write(2, color, ft_strlen(color));
	write(2, msg, ft_strlen(msg));
	write(2, RESET, ft_strlen(RESET));
	if (exit_status)
		free_and_exit(exit_status);
}

uint8_t	print_first_check_error(char *msg, char *token,
		char **envp[], char **shell_prompt)
{
	if (!token)
	{
		print_message(msg, RED, 0);
		print_message("\n", RED, 0);
	}
	else
	{
		print_message(msg, RED, 0);
		print_message(" `", RED, 0);
		print_message(token, RED, 0);
		print_message("'.\n", RED, 0);
	}
	g_status = 2;
	*shell_prompt = create_prompt(envp);
	return (1);
}

void	errno_file_error(char *file, int exit_status)
{
	write(2, RED, ft_strlen(RED));
	write(2, "Minishell: ", 11);
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	write(2, ": ", 2);
	write(2, file, ft_strlen(file));
	write(2, RESET, ft_strlen(RESET));
	write(2, "\n", 1);
	g_status = exit_status;
}

int	print_dirpath_err(char *path)
{
	ft_putstr_fd("minishell: cd: ", 2);
	if (access(path, F_OK) == -1)
		ft_putstr_fd("no such file or directory: ", 2);
	else if (access(path, R_OK) == -1)
		ft_putstr_fd("permission denied: ", 2);
	else
		ft_putstr_fd("not a directory: ", 2);
	ft_putendl_fd(path, 2);
	return (1);
}
