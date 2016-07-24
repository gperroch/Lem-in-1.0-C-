# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gperroch <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/03/15 08:10:39 by gperroch          #+#    #+#              #
#    Updated: 2016/04/08 19:50:26 by gperroch         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= lem-in

SRC_NAMES	= cleaning.c			\
			  main.c				\
			  initialisation.c		\
			  initialisation_2.c	\
			  parsing.c				\
			  parsing_2.c			\
			  algo.c				\
			  mapping.c				\
			  deplacement.c
OBJ_NAMES	= $(SRC_NAMES:.c=.o)
INC_NAMES	= lem_in.h				\
			  libftprintf.h
LIB_NAMES	= libftprintf.a

SRC_PATH	= ./SOURCES
OBJ_PATH	= ./OBJECTS
INC_PATH	= ./INCLUDES
LIB_PATH	= ./libftprintf

SRC			= $(addprefix $(SRC_PATH)/,$(SRC_NAMES))
OBJ			= $(addprefix $(OBJ_PATH)/,$(OBJ_NAMES))
LIB			= $(addprefix $(LIB_PATH)/,$(LIB_NAMES))

FLAGS		= -Wall -Wextra -Werror

all			: $(NAME)

$(NAME)		: $(OBJ)
	@make -C $(LIB_PATH) && make clean -C $(LIB_PATH)
	@gcc $(FLAGS) $(LIB) $(OBJ) -o $@

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir $(OBJ_PATH) 2> /dev/null || true
	@gcc $(FLAGS) -I$(INC_PATH) -o $@ -c $<

clean		:
	@rm -f $(OBJ)
	@rmdir $(OBJ_PATH) 2> /dev/null || true

fclean		: clean
	@rm -f lem-in

re			: fclean all

norme		:
	@norminette $(SRC_PATH)
	@norminette $(INC_PATH)
