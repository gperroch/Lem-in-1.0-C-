/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mapping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperroch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/02 10:07:26 by gperroch          #+#    #+#             */
/*   Updated: 2016/04/06 18:21:46 by gperroch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void		ft_mapping(t_data *data)
{
	t_room	*cursor_room;
	int		i;

	cursor_room = data->start;
	while (cursor_room)
	{
		i = -1;
		cursor_room->map = (int*)malloc(sizeof(int) * cursor_room->nb_link);
		while (++i < cursor_room->nb_link)
			cursor_room->map[i] = ftmapping_index(data, cursor_room->link[i],
					cursor_room);
		cursor_room = cursor_room->init_order_next;
	}
}

static void	ftmapping_index_auxi(t_data *data, t_room *cursor1,
		t_room *prev, int *curr_index)
{
	int		i;

	while (!data->end->index)
	{
		i = -1;
		while (cursor1->link && cursor1->link[++i] && cursor1->index)
		{
			if (!cursor1->link[i]->index && cursor1->index == *curr_index
					&& ft_strcmp(cursor1->link[i]->name, prev->name))
				cursor1->link[i]->index = *curr_index + 1;
		}
		if (cursor1->init_order_next)
			cursor1 = cursor1->init_order_next;
		else
		{
			(*curr_index)++;
			cursor1 = data->start;
		}
	}
}

int			ftmapping_index(t_data *data, t_room *room, t_room *prev)
{
	t_room	*cursor1;
	int		curr_index;
	int		ret;

	if (!ft_strcmp(room->name, data->end->name))
		return (0);
	if (!ft_strcmp(prev->name, data->end->name) || room->nb_link == 1
			|| !ftmapping_no_way(data, room, prev))
		return (-1);
	cursor1 = room;
	curr_index = 1;
	cursor1->index = curr_index;
	ftmapping_index_auxi(data, cursor1, prev, &curr_index);
	ret = data->end->index;
	ft_indexage(data);
	return (ret - 1);
}

int			ftmapping_no_way(t_data *data, t_room *room, t_room *prev)
{
	prev->index = 1;
	ftmapping_no_way_recu(room);
	if (!data->end->index)
		return (0);
	ft_indexage(data);
	return (1);
}

void		ftmapping_no_way_recu(t_room *room)
{
	t_room	*next;
	int		i;

	if (!room->link)
		return ;
	i = -1;
	if (!room->index)
		room->index = 1;
	while ((next = room->link[++i]))
	{
		if (next->index != 1)
			ftmapping_no_way_recu(next);
	}
}
