# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lcottet <lcottet@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/14 13:24:31 by bwisniew          #+#    #+#              #
#    Updated: 2025/11/06 21:12:36 by lcottet          ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS = -Wall -Wextra -Werror -MMD -MP -g3 -fsanitize=address

SRCS_DIR = srcs

SRCS =	main.c

OUTDIR = obj

OBJ = $(SRCS:%.c=$(OUTDIR)/%.o)

DEP = $(OBJ:%.o=%.d)

INCLUDE = includes libs/libft libs/vector/includes

NAME = ft_nm

LIBFT = libs/libft/libft.a

VECTOR = libs/vector/libvct.a
all: $(NAME)

$(NAME): $(OBJ) $(LIBFT) $(VECTOR)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(VECTOR)

$(OUTDIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE:%=-I %) -o $@ -c $<

$(LIBFT): FORCE
	make -C libs/libft

$(VECTOR): FORCE
	make -C libs/vector

clean:
	rm -rf $(OUTDIR)
	make -C libs/libft clean
	make -C libs/vector clean

fclean: clean
	rm -f $(NAME) $(LIBFT) $(VECTOR)

re: fclean $(NAME)

FORCE:

-include $(DEP)

.PHONY: all clean fclean re norm FORCE