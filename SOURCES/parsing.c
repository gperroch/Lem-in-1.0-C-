/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperroch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/23 10:16:08 by gperroch          #+#    #+#             */
/*   Updated: 2016/04/08 10:54:54 by gperroch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void			ft_error(int err, t_data *data)
{
	if (err == LINK_MISS)
		ft_putendl("Link missing.");
	if (err == EMPTY_FILE)
		ft_putendl("The file is empty.");
	if (err == ANT_MISS)
		ft_putendl("Ants missing.");
	ft_cleaning(&data);
	exit(1);
}

int				ft_end_reading(t_data *data, char *line, char *prev)
{
	int			only_numbers;
	char		*tmp;
	t_room		*cursor;
	int			ret;

	only_numbers = 0;
	cursor = data->room;
	tmp = NULL;
	ft_record_line(data, line);
	if (ft_strstr(line, " ") && line[0] != '#')
		tmp = ft_strdupchr(line, ' ');
	while (cursor && tmp && ft_strcmp(cursor->name, tmp))
		cursor = cursor->init_order_next;
	if (tmp && cursor)
	{
		free(tmp);
		return (-1);
	}
	free(tmp);
	while (only_numbers >= 0 && line[only_numbers])
		only_numbers = (ft_isdigit(line[only_numbers++])) ? only_numbers : -1;
	ret = (line && only_numbers == -1 && data->nb_ant == -1) ? -1 : 0;
	ret = (ft_end_auxi(data, line, prev)) ? -1 : ret;
	ret = (line[0] == '#') ? 0 : ret;
	return (ret);
}

int				ft_check_link(char *line, t_data *data)
{
	char		*link1;
	char		*link2;
	t_room		*cursor;
	int			res;

	res = 2;
	cursor = data->room;
	link1 = ft_strdupchr(line, '-');
	link2 = ft_strdup(ft_strstr(line, "-") + 1);
	while (cursor && res)
	{
		if ((link1 && !ft_strcmp(link1, cursor->name)) ||
				(link2 && !ft_strcmp(link2, cursor->name)))
			res--;
		cursor = cursor->init_order_next;
	}
	res = (!res) ? 0 : -1;
	free(link1);
	free(link2);
	return (res);
}

int				ft_check_data(t_data *data)
{
	int			ret;

	ret = 0;
	ret = (data->nb_ant > 0) ? 0 : -1;
	ret = (data->init_order_link) ? ret : -1;
	ret = (data->start) ? ret : -1;
	ret = (data->end) ? ret : -1;
	ret = (data->room) ? ret : -1;
	ret = (data->init_order_link) ? ret : -1;
	if (ft_check_coor(data) == -1)
		ret = -1;
	if (ret == -1)
		ft_putendl("Error.");
	return (ret);
}

int				ft_check_coor(t_data *data)
{
	t_room		*cursor1;
	t_room		*cursor2;

	cursor1 = data->room;
	while (cursor1)
	{
		cursor2 = cursor1->init_order_next;
		while (cursor2)
		{
			if (cursor1->x == cursor2->x && cursor1->y == cursor2->y)
				return (-1);
			cursor2 = cursor2->init_order_next;
		}
		cursor1 = cursor1->init_order_next;
	}
	return (0);
}
