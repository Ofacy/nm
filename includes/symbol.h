#ifndef SYMBOL_H
# define SYMBOL_H

# include <elf.h>

typedef struct s_symbol {
	char	address[17];
	char	*name;
	int		name_len;
	char	identifier;
}	t_symbol;

char get_identifier(Elf64_Sym sym, Elf64_Shdr *shdr, size_t shnum);

int should_print_address(char identifier);

int is_filtered_symbol(char identifier);

void print_symbol(t_symbol *symbol);

#endif
