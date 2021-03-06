# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rmicolon <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/01/05 11:15:04 by rmicolon          #+#    #+#              #
#    Updated: 2016/08/04 23:37:39 by rmicolon         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc

FLAGS = -Wall \
		-Wextra \
		-Werror \
		-g

NAME = corewar

SDIR = sources

ODIR = objects

SRCS = $(SDIR)/corewar.c \
		$(SDIR)/display.c \
		$(SDIR)/init_proc.c \
		$(SDIR)/error.c \
		$(SDIR)/fork.c \
		$(SDIR)/game_handling.c \
		$(SDIR)/destroy_process.c \
		$(SDIR)/livenstore.c \
		$(SDIR)/loads.c \
		$(SDIR)/find_instructions.c \
		$(SDIR)/display_mem.c \
		$(SDIR)/display_misc.c \
		$(SDIR)/or_and_xor.c \
		$(SDIR)/display_info_tab.c \
		$(SDIR)/ins_tools_one.c \
		$(SDIR)/ins_tools_two.c \
		$(SDIR)/ins_update_pc.c \
		$(SDIR)/add_sub_aff.c \
		$(SDIR)/cw_misc.c \
		$(SDIR)/cw_init.c \
		$(SDIR)/cw_add_player.c \

OBJ = $(SRCS:$(SDIR)/%.c=$(ODIR)/%.o)

HDRS = -I./includes \
	   -I./libft/includes

LIBFT = ./libft/libft.a

LIBS = -L./libft/ -lft -lncurses

all : $(NAME)

$(NAME): $(ODIR) $(LIBFT) $(OBJ)
	@$(CC) $(FLAGS) $(LIBS) $(OBJ) -o $@
	@echo "corewar created."

$(ODIR):
	@mkdir objects

$(ODIR)/%.o: $(SDIR)/%.c
	@$(CC) $(FLAGS) -c $< $(HDRS) -o $@
	@echo "Object created."

$(LIBFT):
	@make -C libft/

clean:
	@rm -rf $(ODIR)
	@make clean -C libft
	@echo "Objects deleted."

fclean: clean
	@rm -f $(NAME)
	@make fclean -C libft
	@echo "Executable deleted."

re: fclean all

.PHONY: all clean fclean re
