/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperroch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/15 08:26:23 by gperroch          #+#    #+#             */
/*   Updated: 2016/04/07 10:30:16 by gperroch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void					ft_cleaning(t_data **data)
{
	t_data				*ptr;

	ptr = *data;
	if (!ptr)
		return ;
	ft_free_cortege(ptr);
	ft_free_room(ptr->room);
	ft_freetab_str(ptr->init_order_link, -1);
	free(ptr->init_order_link);
	if (ptr->best_ways && *(ptr->best_ways))
		ft_freetab_str(ptr->best_ways, -1);
	if (ptr->best_ways)
		free(ptr->best_ways);
	if (ptr->text)
		free(ptr->text);
	free(ptr);
}

void					ft_free_room(t_room *room)
{
	t_room				*tmp;

	while (room)
	{
		tmp = room->init_order_next;
		free(room->name);
		free(room->link);
		if (room->map)
			free(room->map);
		free(room);
		room = tmp;
	}
}

void					ft_free_cortege(t_data *data)
{
	t_cortege			*cortege;
	t_cortege			*tmp;

	cortege = data->cortege;
	while (cortege)
	{
		tmp = cortege->next;
		free(cortege);
		cortege = tmp;
	}
}
