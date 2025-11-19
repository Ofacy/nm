
#include <stdlib.h>
#include <elf.h>
#include "symbol.h"
#include "libft.h"

char get_identifier(Elf64_Sym sym, void *shdr, t_arch_functions arch_specifics, size_t shnum)
{
	char identifier = '?';

	switch (sym.st_shndx) {
		case SHN_UNDEF:
			identifier = 'U';
			break;
		case SHN_ABS:
			identifier = 'A';
			break;
		case SHN_COMMON:
			identifier = 'C';
			break;
		default:
			if (sym.st_shndx < shnum) {
				Elf64_Shdr sec = arch_specifics.get_section_header(shdr, sym.st_shndx);
				if (sec.sh_type == SHT_NOBITS && (sec.sh_flags & SHF_ALLOC) && (sec.sh_flags & SHF_WRITE))
					identifier = 'B';
				else if (sec.sh_type == SHT_PROGBITS && (sec.sh_flags & SHF_ALLOC) && (sec.sh_flags & SHF_EXECINSTR))
					identifier = 'T';
				else if ((sec.sh_flags & SHF_ALLOC) && (sec.sh_flags & SHF_WRITE))
					identifier = 'D';
				else if (sec.sh_flags & SHF_ALLOC)
					identifier = 'R';
				else if (sec.sh_type == SHT_GROUP)
					identifier = 'n';
				else
					identifier = 'N';
			}
			if (ELF64_ST_TYPE(sym.st_info) == STT_GNU_IFUNC) {
				return 'i';
			}
			break;
	}
	switch (ELF64_ST_BIND(sym.st_info)) {
		case STB_LOCAL:
			if (identifier != '?' && identifier != 'n' && identifier != 'N')
				identifier += 32;
			break;
		case STB_WEAK:
			if (ELF64_ST_TYPE(sym.st_info) == STT_OBJECT) {
				if (identifier == 'U')
					identifier = 'v';
				else
					identifier = 'V';
				break;
			}
			if (identifier == 'U')
				identifier = 'w';
			else
				identifier = 'W';
			break;
	}
	return identifier;
}

int should_print_address(char identifier)
{
	return (identifier != 'u' && identifier != 'U' && identifier != 'w' && identifier != 'v');
}

int is_filtered_symbol(t_symbol symbol)
{
	if (symbol.name == NULL)
		return (1);

	return (symbol.identifier == '?' || symbol.identifier == 'n' || symbol.identifier == 'N' || symbol.identifier == 'a');
}

void print_symbol(t_symbol *symbol) {
	if (ft_printf("%s %c", symbol->address_str, symbol->identifier) == -1)
		return ;
	if (symbol->name) {
		if (symbol->name[symbol->name_len] != '\0')
			symbol->name = NULL;
		if (ft_printf(" %s", symbol->name) == -1)
			return ;
	}
	if (ft_printf("\n") == -1)
		return ;
	return ;
}