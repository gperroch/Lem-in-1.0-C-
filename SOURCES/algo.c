/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperroch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/17 16:01:39 by gperroch          #+#    #+#             */
/*   Updated: 2016/04/08 10:25:28 by gperroch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void		ft_algo(t_data *data)
{
	t_cortege	*cursor_ant;

	data->cortege->prev_room = data->start;
	ft_check_validity(data, 1);
	ft_replace_end(data);
	ft_check_validity(data, 2);
	cursor_ant = data->cortege;
	ft_indexage(data);
	ft_putendl(data->text);
	ft_mapping(data);
	while (data->end->nb_ant < data->nb_ant)
	{
		ft_deplacement_mapping(data, cursor_ant);
		if (ft_strcmp(cursor_ant->prev_room->name, cursor_ant->room->name))
		{
			ft_printf("L%d-%s", cursor_ant->num, cursor_ant->room->name);
			if (data->end->nb_ant != data->nb_ant)
				write(1, " ", 1);
		}
		if (!cursor_ant->next)
			ft_putstr("\n");
		cursor_ant = (cursor_ant->next) ? cursor_ant->next : data->cortege;
	}
}

void		ft_check_validity(t_data *data, int nb)
{
	t_room	*cursor;

	cursor = data->start;
	ft_primal_way(cursor, nb);
	if (data->end->index != nb)
	{
		ft_putstr("Error.");
		ft_cleaning(&data);
		exit(1);
	}
}

void		ft_primal_way(t_room *room, int nb)
{
	t_room	*next;
	int		i;

	if (!room->link)
		return ;
	i = -1;
	room->index = nb;
	while ((next = room->link[++i]))
	{
		if (next->index != nb)
			ft_primal_way(next, nb);
	}
}

void		ft_indexage(t_data *data)
{
	t_room	*cursor;
	int		i;

	i = 0;
	cursor = data->start;
	cursor->index = i;
	while ((cursor = cursor->init_order_next))
		cursor->index = i;
}

void		ft_replace_end(t_data *data)
{
	t_room	*cursor;

	if (!data->end->init_order_next)
		return ;
	data->end->init_order_previous->init_order_next =
		data->end->init_order_next;
	data->end->init_order_next->init_order_previous =
		data->end->init_order_previous;
	cursor = data->start;
	while (cursor->init_order_next)
		cursor = cursor->init_order_next;
	cursor->init_order_next = data->end;
	data->end->init_order_previous = cursor;
	data->end->init_order_next = NULL;
}
