/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deplacement.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperroch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/08 11:14:18 by gperroch          #+#    #+#             */
/*   Updated: 2016/04/08 11:14:20 by gperroch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void		ft_deplacement_mapping(t_data *data, t_cortege *ant)
{
	int		start_index;
	int		index;

	if (!ft_strcmp(ant->room->name, data->end->name))
		ant->prev_room = data->end;
	if (!ft_strcmp(ant->room->name, data->end->name))
		return ;
	index = ft_find_index_start(ant->room->map, ant->room->nb_link);
	start_index = index;
	while (index != -1)
	{
		if ((ant->room->link[index]->nb_ant == 0
		&& ((ant->room->map[index] - ant->room->map[start_index]
		< data->nb_ant - ant->num)
			|| ant->room->map[start_index] == ant->room->map[index])
		&& ft_strcmp(ant->prev_room->name, ant->room->link[index]->name))
		|| ant->room->map[index] == 0)
		{
			ft_moove_mapping(ant, index);
			return ;
		}
		else
			index = ft_find_index(ant->room->map, ant->room->nb_link, index);
	}
	ant->prev_room = ant->room;
}

static int	ft_find_index_auxi(int *map, int length, int last_value,
		int last_index)
{
	int		i;
	int		palier;
	int		ret;

	i = -1;
	palier = last_value + 1;
	ret = -1;
	while (++i < length)
	{
		if (map[i] >= palier && (ret == -1 || map[i] < map[ret])
				&& map[i] != -1)
			ret = i;
	}
	if (i == length && ret == last_index)
		ret = -1;
	return (ret);
}

int			ft_find_index(int *map, int length, int last_index)
{
	int		i;
	int		last_value;
	int		ret;
	int		palier;

	i = -1;
	palier = -1;
	ret = last_index;
	last_value = map[last_index];
	while (++i < length)
	{
		if (map[i] == last_value && i > last_index && map[i] > -1)
			ret = i;
	}
	if (i == length && ret == last_index)
		ret = ft_find_index_auxi(map, length, last_value, last_index);
	return (ret);
}

int			ft_find_index_start(int *map, int length)
{
	int		i;
	int		ret;

	i = -1;
	ret = 0;
	while (++i < length)
	{
		if ((map[i] < map[ret] || map[ret] == -1) && map[i] > -1)
			ret = i;
	}
	return (ret);
}

void		ft_moove_mapping(t_cortege *ant, int index)
{
	t_room	*prev;

	prev = ant->room;
	ant->prev_room = prev;
	ant->room = prev->link[index];
	prev->nb_ant--;
	ant->room->nb_ant++;
	prev->ant = NULL;
	ant->room->ant = ant;
}
