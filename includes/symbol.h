#ifndef SYMBOL_H
# define SYMBOL_H

# include <elf.h>
# include "nm.h"

typedef struct s_symbol {
	uint64_t	name_offset;
	size_t		og_index;
	char		*name;
	char		address_str[17];
	int			name_len;
	uint16_t	section_index;
	char		info;
	char		identifier;
}	t_symbol;

char get_identifier(Elf64_Sym sym, void *shdr, t_arch_functions arch_specifics, size_t shnum);

int should_print_address(char identifier);

int is_filtered_symbol(t_symbol symbol);
int is_defined_symbol(t_symbol symbol);
int is_invalid_symbol(t_symbol symbol);
int is_external_symbol(t_symbol symbol);

void print_symbol(t_symbol *symbol);


void get_file_options(
	int options,
	int (**is_filtered)(t_symbol),
	int (**compare)(t_symbol *, t_symbol *));

#endif
