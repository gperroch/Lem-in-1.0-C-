/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperroch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/14 18:18:17 by gperroch          #+#    #+#             */
/*   Updated: 2016/04/08 11:19:09 by gperroch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H

# include "libftprintf.h"
# define LINK_MISS 404
# define EMPTY_FILE 405
# define ANT_MISS 406

typedef struct			s_data
{
	int					nb_ant;
	int					nb_room;
	char				*text;
	char				**init_order_link;
	char				**best_ways;
	struct s_room		*start;
	struct s_room		*end;
	struct s_cortege	*cortege;
	struct s_room		*room;
}						t_data;

typedef struct			s_cortege
{
	int					num;
	char				**best_ways;
	struct s_room		*room;
	struct s_room		*prev_room;
	struct s_cortege	*next;
	struct s_cortege	*previous;
}						t_cortege;

typedef struct			s_room
{
	int					x;
	int					y;
	int					index;
	char				*name;
	int					nb_ant;
	int					nb_link;
	struct s_cortege	*ant;
	struct s_room		*init_order_next;
	struct s_room		*init_order_previous;
	struct s_room		**link;
	int					*map;
}						t_room;

typedef struct			s_node
{
	char				*way;
	int					way_length;
	struct s_room		*room;
	struct s_node		*previous;
}						t_node;

void					ft_cleaning(t_data **data);
void					ft_free_cortege(t_data *data);
void					ft_free_room(t_room *room);
void					ft_initialisation(t_data **data);
void					ft_allocation(t_data **data);
int						ft_distribution(t_data *data, char *line,
		char *previous_line);
t_room					*ft_new_room(char *line);
void					ft_add_init_order_room(t_data *data, t_room *room);
void					ft_add_init_order_link(t_data *data, char *line);
void					ft_update_order(t_data *data);
void					ft_add_link(t_room *room_a, t_room *room_b);
void					ft_algo(t_data *data);
void					ft_error(int err, t_data *data);
int						ft_end_reading(t_data *data, char *line, char *prev);
int						ft_end_auxi(t_data *data, char *line, char *prev);
int						ft_check_data(t_data *data);
int						ft_check_link(char *line, t_data *data);
int						ft_check_coor(t_data *data);
void					ft_check_validity(t_data *data, int nb);
void					ft_primal_way(t_room *room, int nb);
void					ft_cortege_creation(t_data *data);
void					ft_indexage(t_data *data);
void					ft_replace_end(t_data *data);
void					ft_erase_cr(char **line);
void					ft_record_line(t_data *data, char *line);
void					ft_mapping(t_data *data);
int						ftmapping_index(t_data *data, t_room *room,
		t_room *prev);
int						ftmapping_no_way(t_data *data, t_room *room,
		t_room *prev);
void					ftmapping_no_way_recu(t_room *room);
void					ft_deplacement_mapping(t_data *data, t_cortege *ant);
void					ft_moove_mapping(t_cortege *ant, int index);
int						ft_find_index(int *map, int length, int last_index);
int						ft_find_index_start(int *map, int length);

#endif
