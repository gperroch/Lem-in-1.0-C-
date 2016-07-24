/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisation_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperroch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/23 10:14:42 by gperroch          #+#    #+#             */
/*   Updated: 2016/04/06 09:52:19 by gperroch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

t_room			*ft_new_room(char *line)
{
	t_room		*new_room;
	char		*coor;

	coor = ft_strstr(line, " ") + 1;
	if (!(new_room = (t_room*)malloc(sizeof(t_room))))
		exit(1);
	new_room->init_order_next = NULL;
	new_room->init_order_previous = NULL;
	new_room->link = NULL;
	new_room->name = ft_strdupchr(line, ' ');
	new_room->ant = NULL;
	new_room->nb_ant = 0;
	new_room->x = ft_atoi(coor);
	new_room->y = ft_atoi(ft_strstr(coor, " ") + 1);
	new_room->index = 0;
	new_room->map = NULL;
	new_room->nb_link = 0;
	return (new_room);
}

void			ft_add_init_order_room(t_data *data, t_room *room)
{
	t_room		*cursor;

	data->nb_room++;
	if (!data->room)
	{
		data->room = room;
		return ;
	}
	cursor = data->room;
	while (cursor->init_order_next)
		cursor = cursor->init_order_next;
	cursor->init_order_next = room;
	room->init_order_previous = cursor;
}

void			ft_add_init_order_link(t_data *data, char *line)
{
	if (data->init_order_link == NULL)
	{
		data->init_order_link = (char**)malloc(sizeof(char*) * 2);
		data->init_order_link[0] = ft_strdup(line);
		data->init_order_link[1] = NULL;
		return ;
	}
	ft_tabjoin_str(&data->init_order_link, -1, line);
}

void			ft_cortege_creation(t_data *data)
{
	int			nb_ant;
	t_cortege	*cursor;
	t_cortege	*new_ant;

	nb_ant = data->nb_ant;
	cursor = data->cortege;
	while (--nb_ant)
	{
		if (!(new_ant = (t_cortege*)malloc(sizeof(t_cortege))))
			exit(EXIT_FAILURE);
		new_ant->num = data->nb_ant - nb_ant + 1;
		new_ant->best_ways = NULL;
		new_ant->room = data->start;
		new_ant->prev_room = data->start;
		new_ant->next = NULL;
		new_ant->previous = cursor;
		cursor->next = new_ant;
		cursor = cursor->next;
	}
}
