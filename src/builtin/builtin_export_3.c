/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 03:29:34 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/25 15:08:41 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_export(t_list	*tmp, int fd)
{
	ft_putstr_fd("export ", fd);
	ft_putstr_fd(((t_export *)tmp->content)->var, fd);
	if (((t_export *)tmp->content)->equal)
	{
		ft_putstr_fd("=", fd);
		ft_putstr_fd("\"", fd);
	}
	ft_putstr_fd(((t_export *)tmp->content)->value, fd);
	if (((t_export *)tmp->content)->equal)
		ft_putendl_fd("\"", fd);
	else
		ft_putendl_fd("", fd);
}

void	print_strs_fd(t_list **export_list, int fd)
{
	t_list	*tmp;
	int		i;

	i = 0;
	lst_bubblesort(export_list);
	tmp = *export_list;
	while (tmp)
	{
		if (!ft_strcmp(((t_export *)tmp->content)->var, "_")
			&& ((t_export *)tmp->content)->equal)
		{
			tmp = tmp->next;
			continue ;
		}
		print_export(tmp, fd);
		tmp = tmp->next;
	}
}

void	ft_lst_swap_content(t_list *el_1, t_list *el_2)
{
	void	*tmp;

	tmp = el_1->content;
	el_1->content = el_2->content;
	el_2->content = tmp;
}

void	lst_bubblesort(t_list **list_to_sort)
{
	t_list	*tmp;
	t_list	*tmp_sort;

	tmp = *list_to_sort;
	while (tmp != NULL)
	{
		tmp_sort = tmp->next;
		while (tmp_sort != NULL)
		{
			if (ft_strcmp(((t_export *)tmp_sort->content)->var,
					((t_export *)tmp->content)->var) < 0)
				ft_lst_swap_content(tmp, tmp_sort);
			tmp_sort = tmp_sort->next;
		}
		tmp = tmp->next;
	}
}
