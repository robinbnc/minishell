/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:30:10 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/25 16:43:30 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static uint8_t	is_wildcard(char *arg)
{
	while (*arg)
	{
		if (*arg == '"')
			arg += quote_len(&(*arg), '"');
		else if (*arg == '\'')
			arg += quote_len(&(*arg), '\'');
		else if (*arg == '*' || *arg == '?')
			return (1);
		arg++;
	}
	return (0);
}

static char	*get_match(char *arg, char *file, int arg_i, int file_i)
{
	while (arg[arg_i] && file[file_i])
	{
		while (file[file_i] && arg[arg_i] != '*')
		{
			if (arg[arg_i++] != file[file_i++] && arg[arg_i - 1] != '?')
				return ("");
		}
		while (arg[arg_i] == '*')
			arg_i++;
		if (arg[arg_i - 1] == '*' && arg[arg_i])
		{
			while (is_star(file, arg, arg_i, file_i))
				file_i++;
			if (!file[file_i])
				return ("");
			file_i += ft_strlento(&arg[arg_i], '*');
			arg_i += ft_strlento(&arg[arg_i], '*');
		}
		else if (!arg[arg_i])
			return (ft_strjoin(file, " ", LOOP));
	}
	if (!arg[arg_i] && !file[file_i])
		return (ft_strjoin(file, " ", LOOP));
	return ("");
}

static char	*read_dir(void *dir, char *tmp, char *arg)
{
	char			*narg;
	struct dirent	*s_dir;

	narg = "";
	s_dir = readdir(dir);
	while (s_dir)
	{
		if (s_dir->d_name[0] != '.')
		{
			tmp = get_match(arg, s_dir->d_name, 0, 0);
			narg = ft_strjoin(narg, tmp, LOOP);
		}
		if (!narg || !tmp)
			print_message("minishell: Allocation error\n", RED, 1);
		s_dir = readdir(dir);
	}
	closedir(dir);
	return (narg);
}

static char	*get_dir_infos(char *arg)
{
	DIR				*dir;
	char			*narg;
	char			*tmp;

	if (!is_wildcard(arg))
		return (NULL);
	tmp = garbage_addptr(getcwd(NULL, 0), LOOP);
	if (!tmp)
		print_message("minishell: Allocation error.\n", RED, 1);
	dir = opendir(tmp);
	if (dir)
	{
		narg = read_dir(dir, tmp, arg);
		if (narg[0] == '\0')
			return (arg);
		if (narg)
		{
			narg = ft_sort_strarr(narg, LOOP);
			if (!narg)
				print_message("minishell: Allocation error\n", RED, 1);
		}
		return (narg);
	}
	return (NULL);
}

void	wildcard(t_list *command_list)
{
	t_pipe_command	*command;
	char			*to_insert;
	int				argc;

	while (command_list)
	{
		argc = 0;
		command = (t_pipe_command *)command_list->content;
		while (command->exec_args[argc])
		{
			to_insert = get_dir_infos(command->exec_args[argc]);
			if (to_insert)
				command->exec_args[argc] = to_insert;
			argc++;
		}
		command_list = command_list->next;
	}
}
