/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperroch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/15 08:20:54 by gperroch          #+#    #+#             */
/*   Updated: 2016/04/08 10:27:00 by gperroch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void			ft_initialisation(t_data **data)
{
	char		*line;
	char		*previous_line;
	int			ret_dist;

	ret_dist = 1;
	previous_line = ft_strnew(0);
	ft_allocation(data);
	while (get_next_line(0, &line) > 0 && ret_dist > 0)
	{
		ft_erase_cr(&line);
		ret_dist = ft_distribution(*data, line, previous_line);
		free(previous_line);
		previous_line = ft_strdup(line);
		free(line);
	}
	if (previous_line)
		free(previous_line);
	if (ft_check_data(*data) == -1)
		ft_error(0, *data);
	(*data)->start->ant = (*data)->cortege;
	(*data)->start->nb_ant = (*data)->nb_ant;
	(*data)->cortege->room = (*data)->start;
	(*data)->end->ant = 0;
	ft_update_order(*data);
	ft_cortege_creation(*data);
}

void			ft_allocation(t_data **data)
{
	if (!(*data))
		*data = (t_data*)malloc(sizeof(t_data));
	if (!(*data))
		exit(1);
	(*data)->nb_ant = -1;
	(*data)->init_order_link = NULL;
	(*data)->start = NULL;
	(*data)->end = NULL;
	(*data)->cortege = (t_cortege*)malloc(sizeof(t_cortege));
	(*data)->room = NULL;
	if (!(*data)->cortege)
		exit(1);
	(*data)->cortege->num = 1;
	(*data)->cortege->room = NULL;
	(*data)->cortege->next = NULL;
	(*data)->cortege->previous = NULL;
	(*data)->cortege->best_ways = NULL;
	(*data)->best_ways = NULL;
	(*data)->nb_room = 0;
	(*data)->text = NULL;
}

int				ft_distribution(t_data *data, char *line, char *previous_line)
{
	t_room		*tmp_room;

	if (ft_end_reading(data, line, previous_line))
		return (-1);
	if (data->nb_ant == -1 && line[0] != '#')
		data->nb_ant = ft_atoi(line);
	else if (!ft_strcmp(previous_line, "##start"))
	{
		tmp_room = data->room;
		data->room = ft_new_room(line);
		data->start = data->room;
		data->nb_room++;
		if (tmp_room)
			data->room->init_order_next = tmp_room;
	}
	else if (ft_strstr(line, " ") && *line != 'L' && *line != '#')
	{
		tmp_room = ft_new_room(line);
		ft_add_init_order_room(data, tmp_room);
		if (!ft_strcmp(previous_line, "##end"))
			data->end = tmp_room;
	}
	else if (ft_strstr(line, "-"))
		ft_add_init_order_link(data, line);
	return (1);
}

void			ft_update_order(t_data *data)
{
	char		*a_room;
	char		*b_room;
	int			i;
	t_room		*tmp_a;
	t_room		*tmp_b;

	i = -1;
	if (!data->init_order_link)
		ft_error(LINK_MISS, data);
	while ((data->init_order_link)[++i])
	{
		a_room = ft_strdupchr((data->init_order_link)[i], '-');
		b_room = ft_strdup(ft_strstr((data->init_order_link)[i], "-") + 1);
		tmp_a = data->room;
		tmp_b = data->room;
		while (tmp_a && ft_strcmp(tmp_a->name, a_room))
			tmp_a = tmp_a->init_order_next;
		while (tmp_b && ft_strcmp(tmp_b->name, b_room))
			tmp_b = tmp_b->init_order_next;
		ft_add_link(tmp_a, tmp_b);
		ft_add_link(tmp_b, tmp_a);
		free(a_room);
		free(b_room);
	}
}

void			ft_add_link(t_room *room_a, t_room *room_b)
{
	int			len;
	t_room		**new_links;

	len = 0;
	if (!room_a || !room_b)
		return ;
	room_a->nb_link++;
	if (!room_a->link)
	{
		if (!(room_a->link = (t_room**)malloc(sizeof(t_room*) * 2)))
			exit(1);
		room_a->link[0] = room_b;
		room_a->link[1] = NULL;
		return ;
	}
	while (room_a->link[len])
		len++;
	if (!(new_links = (t_room**)malloc(sizeof(t_room*) * (len + 2))))
		exit(1);
	new_links[len + 1] = NULL;
	new_links[len] = room_b;
	while (--len >= 0)
		new_links[len] = room_a->link[len];
	free(room_a->link);
	room_a->link = new_links;
}
