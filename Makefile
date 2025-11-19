# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lcottet <lcottet@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/14 13:24:31 by bwisniew          #+#    #+#              #
#    Updated: 2025/11/19 18:03:06 by lcottet          ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS = -Wall -Wextra -Werror -MMD -MP -g3

SRCS_DIR = srcs

SRCS =	main.c identifier.c

OUTDIR = obj

OBJ = $(SRCS:%.c=$(OUTDIR)/%.o)

DEP = $(OBJ:%.o=%.d)

INCLUDE = includes libs/libft libs/vector/includes

NAME = ft_nm

LIBFT = libs/libft/libft.a

VECTOR = libs/vector/libvct.a

.PHONY: all
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

.PHONY: clean
clean:
	rm -rf $(OUTDIR)
	make -C libs/libft clean
	make -C libs/vector clean

.PHONY: fclean
fclean: clean
	rm -f $(NAME) $(LIBFT) $(VECTOR)

.PHONY: re
re: fclean $(NAME)

.PHONY: FORCE
FORCE:

-include $(DEP)
