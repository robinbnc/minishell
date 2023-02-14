/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:33:18 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/25 15:22:12 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		mem_remove(arr[i], LOOP);
		i++;
	}
	mem_remove(arr, LOOP);
}

static void	print_error(char *error)
{	
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(error, 2);
	ft_putendl_fd(": No such file or directory", 2);
	free_and_exit(127);
}

static void	set_paths(char **bin_path, char ***path,
	char **envp[], char **exec_args)
{
	*bin_path = get_env("PATH", envp);
	if (!*bin_path)
		print_error(exec_args[0]);
	*path = ft_split(*bin_path, ":", LOOP);
	if (!*path)
		print_message("minishell: Allocation error.\n", RED, 1);
}

char	*get_path(char **exec_args, char **envp[])
{
	int				i;
	char			*bin_path;
	char			**path;
	struct stat		stat;

	path = NULL;
	set_paths(&bin_path, &path, envp, exec_args);
	i = -1;
	while (path && path[++i])
	{
		bin_path = ft_strjoin(path[i], "/", LOOP);
		bin_path = ft_strjoin(bin_path, exec_args[0], LOOP);
		if (!bin_path)
			print_message("minishell: Allocation error.\n", RED, 1);
		if (lstat(bin_path, &stat) == -1)
			mem_remove(bin_path, LOOP);
		else
		{
			free_arr(path);
			return (bin_path);
		}
	}
	free_arr(path);
	return (NULL);
}
