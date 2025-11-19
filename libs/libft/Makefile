NAME = libft.a

OUTDIR = obj

SRCS = ft_isalpha.c \
		ft_isdigit.c \
		ft_isalnum.c \
		ft_isascii.c \
		ft_isprint.c \
		ft_strlen.c \
		ft_memset.c \
		ft_bzero.c \
		ft_memcpy.c \
		ft_memmove.c \
		ft_strlcpy.c \
		ft_strlcat.c \
		ft_tolower.c \
		ft_toupper.c \
		ft_strchr.c \
		ft_strrchr.c \
		ft_strncmp.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_strnstr.c \
		ft_atoi.c \
		ft_calloc.c \
		ft_strdup.c \
		ft_substr.c \
		ft_strjoin.c \
		ft_strtrim.c \
		ft_split.c \
		ft_itoa.c \
		ft_strmapi.c \
		ft_striteri.c \
		ft_putchar_fd.c \
		ft_putstr_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \
		ft_lstnew_bonus.c \
		ft_lstadd_front_bonus.c \
		ft_lstsize_bonus.c \
		ft_lstlast_bonus.c \
		ft_lstadd_back_bonus.c \
		ft_lstdelone_bonus.c \
		ft_lstclear_bonus.c \
		ft_lstiter_bonus.c \
		ft_lstmap_bonus.c \
		get_next_line_bonus.c \
		get_next_line_utils_bonus.c

PRINTF_SRCS = ft_printf/ft_printf.c \
        ft_printf/ft_hex.c \
        ft_printf/ft_number.c \
        ft_printf/ft_char.c

PRINTF_OUTDIR = $(OUTDIR)/ft_printf

HEADERS = ./libft.h

CFLAGS = -Wall -Wextra -Werror -MMD -MP -I ft_printf

OBJECTS = ${SRCS:%.c=$(OUTDIR)/%.o} ${PRINTF_SRCS:%.c=$(OUTDIR)/%.o}

DEPS_FILES = ${OBJECTS:.o=.d}

all: $(NAME)

-include $(DEPS_FILES)

${NAME}: $(OBJECTS)
	ar crs $(NAME) $(OBJECTS)

$(OUTDIR)/%.o: %.c | $(PRINTF_OUTDIR)
	cc $(CFLAGS) -o $@ -c $<

$(PRINTF_OUTDIR):
	mkdir -p $@

clean:
	rm -f $(OBJECTS) $(DEPS_FILES)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re all
