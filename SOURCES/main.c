/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperroch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/15 08:19:19 by gperroch          #+#    #+#             */
/*   Updated: 2016/04/07 09:35:10 by gperroch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

int					main(void)
{
	t_data			*data;

	data = NULL;
	ft_initialisation(&data);
	ft_algo(data);
	ft_cleaning(&data);
	return (0);
}
