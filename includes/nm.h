#ifndef NM_H
# define NM_H

# include <elf.h>
# include <stddef.h>

typedef struct s_arch_functions {
	Elf64_Shdr	(*get_section_header)(void *section_header_array, uint16_t index);
	Elf64_Sym	(*get_symbol)(void *symbol_array, size_t index);
	Elf64_Ehdr	(*get_elf_header)(void *map);
	size_t		sizeof_symbol;
	size_t		sizeof_section_header;
}	t_arch_functions;

Elf64_Shdr	get_section_header_x64(void *section_header_array, uint16_t index);
Elf64_Sym	get_symbol_x64(void *symbol_array, size_t index);
Elf64_Ehdr	get_elf_header_x64(void *map);

Elf64_Shdr	get_section_header_x32(void *section_header_array, uint16_t index);
Elf64_Sym	get_symbol_x32(void *symbol_array, size_t index);
Elf64_Ehdr	get_elf_header_x32(void *map);



#endif
