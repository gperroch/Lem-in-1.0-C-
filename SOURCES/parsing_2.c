/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperroch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/24 15:01:53 by gperroch          #+#    #+#             */
/*   Updated: 2016/04/02 09:32:24 by gperroch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

int			ft_end_auxi(t_data *data, char *line, char *prev)
{
	int		ret;
	int		diff1;
	int		diff2;

	diff1 = ft_strcmp(&line[2], "start");
	diff2 = ft_strcmp(&line[2], "end");
	ret = 0;
	ret = (prev && (!ft_strcmp(prev, "##start") || !ft_strcmp(prev, "##end"))
			&& !ft_strstr(line, " ")) ? -1 : ret;
	ret = (line[0] == '#' && line[1] == '#' && diff1 != 0 && diff1 != 13 &&
			diff2 != 0 && diff2 != 13) ? -1 : ret;
	ret = (ft_strstr(line, "-") && ft_strstr(line, " ")) ? -1 : ret;
	ret = (prev && ((!ft_strstr(line, "-") && ft_strstr(line, " "))
				|| line[0] == '#') && ft_strstr(prev, "-")
			&& !ft_strstr(prev, " ")) ? -1 : ret;
	ret = (ft_strstr(line, "-") && !ft_strstr(line, " ")) ?
		(ft_check_link(line, data)) : ret;
	return (ret);
}

void		ft_erase_cr(char **line)
{
	char	*tmp;
	int		i;

	tmp = *line;
	i = -1;
	while (tmp[++i])
	{
		if (tmp[i] == 13)
			tmp[i] = 0;
	}
}

void		ft_record_line(t_data *data, char *line)
{
	char	*tmp;

	if (!data->text)
		data->text = ft_strjoin(line, "\n");
	else
	{
		tmp = ft_strjoin(data->text, line);
		free(data->text);
		data->text = ft_strjoin(tmp, "\n");
		free(tmp);
	}
}
