/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_step1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 18:24:31 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/25 15:30:56 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_control_op(t_list **lst, char *input)
{
	t_list	*tmp;
	int		i;

	tmp = (*lst)->next;
	i = 0;
	while (tmp)
	{
		while (input[i])
		{
			if (input[i] == '"' || input[i] == '\'')
				i += quote_len(&input[i], input[i]);
			if (control_op_present(tmp, input, &i))
				break ;
			i++;
		}
		tmp = tmp->next;
	}
}

static void	iter_trim(t_list **lst, char *trim)
{
	t_list	*tmp_lst;
	char	*tmp_string;

	tmp_lst = *lst;
	while (tmp_lst)
	{
		tmp_string = ft_strtrim(((t_command *)tmp_lst->content)->command,
				trim, LOOP);
		if (!tmp_string)
			print_message("minishell: Allocation error.\n", RED, 1);
		mem_remove(((t_command *)tmp_lst->content)->command, LOOP);
		((t_command *)tmp_lst->content)->command = tmp_string;
		tmp_lst = tmp_lst->next;
	}
}

static void	parse(char *input, t_list **lst)
{
	char		**splited_input;
	t_command	*tmp;

	splited_input = ft_split_double(input, "||", "&&", LOOP);
	if (!splited_input)
		print_message("minishell: Allocation error.\n", RED, 1);
	while (*splited_input)
	{
		if (mem_alloc(sizeof(t_command), (void **)&tmp, LOOP))
			print_message("minishell: Allocation error.\n", RED, MALLOC_ERR);
		tmp->command = *splited_input;
		tmp->control_op = NULL;
		ft_lstadd_back(lst, ft_lstnew(tmp, LOOP));
		splited_input++;
	}
	set_control_op(lst, input);
}

static void	parse_parenthesis(t_list *command_list)
{
	char	*s;
	int		i;
	int		count;

	count = 0;
	while (command_list)
	{
		i = -1;
		s = ((t_command *)command_list->content)->command;
		while (s[++i] && s[i] == '(')
			count++;
		((t_command *)command_list->content)->nb_parenthesis = count;
		while (s[i] && s[i] != ')')
			i++;
		while (s[i] && s[i] == ')')
		{
			count--;
			i++;
		}
		command_list = command_list->next;
	}
}

t_list	*parse_step1(char *input)
{
	t_list		*command_list;

	if (!*input)
		return (NULL);
	command_list = NULL;
	parse(input, &command_list);
	iter_trim(&command_list, " ");
	parse_parenthesis(command_list);
	iter_trim(&command_list, "() ");
	return (command_list);
}
