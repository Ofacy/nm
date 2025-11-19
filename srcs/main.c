#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <elf.h>
#include <sys/stat.h>

#include <libft.h>
#include <vector.h>

#include "symbol.h"
#include "nm.h"

void fill_addr(char *dest, Elf64_Addr addr, int len)
{
	for (int i = len - 1; i >= 0; i--) {
		int nibble = addr & 0xF;
		if (nibble < 10)
			dest[i] = '0' + nibble;
		else
			dest[i] = 'a' + (nibble - 10);
		addr >>= 4;
	}
}

int get_name_len(const char *strtable, size_t offset, size_t strtable_size)
{
	size_t len = 0;
	while (offset + len < strtable_size && strtable[offset + len] != '\0')
		len++;
	return len;
}

int compare_symbol_names(t_symbol *s1, t_symbol *s2)
{
	if (s1->name == NULL || s2->name == NULL)
		return (s2->name - s1->name);
	if (s1->name_offset != s2->name_offset)
	{
		int strcmp = ft_strncmp(s1->name, s2->name, s1->name_len + 1);
		if (strcmp != 0)
			return (strcmp);
	}
	return (s1->og_index - s2->og_index);
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		return (write(2, "Usage: ./nm <file>\n", 20), 1);
	}
	int fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return (write(2, "Error: Cannot open file\n", 24), 1);

	struct stat st;
	if (fstat(fd, &st) < 0) {
		close(fd);
		return (write(2, "Error: Cannot stat file\n", 24), 1);
	}
	unsigned char *map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (map == MAP_FAILED) {
		close(fd);
		return (write(2, "Error: Cannot map file\n", 23), 1);
	}

	t_arch_functions arch_specifics;
	if (map[EI_MAG0] != ELFMAG0 ||
		map[EI_MAG1] != ELFMAG1 ||
		map[EI_MAG2] != ELFMAG2 ||
		map[EI_MAG3] != ELFMAG3) {
		munmap(map, st.st_size);
		close(fd);
		return (write(2, "Error: Not a valid ELF file\n", 28), 1);
	}
	if (map[EI_CLASS] == ELFCLASS64) {
		arch_specifics.get_section_header = get_section_header_x64;
		arch_specifics.get_symbol = get_symbol_x64;
		arch_specifics.get_elf_header = get_elf_header_x64;
		arch_specifics.sizeof_symbol = sizeof(Elf64_Sym);
		arch_specifics.sizeof_section_header = sizeof(Elf64_Shdr);
		arch_specifics.addr_len = 16;
	}
	else if (map[EI_CLASS] == ELFCLASS32) {
		arch_specifics.get_section_header = get_section_header_x32;
		arch_specifics.get_symbol = get_symbol_x32;
		arch_specifics.get_elf_header = get_elf_header_x32;
		arch_specifics.sizeof_symbol = sizeof(Elf32_Sym);
		arch_specifics.sizeof_section_header = sizeof(Elf32_Shdr);
		arch_specifics.addr_len = 8;
	}
	else
	{
		munmap(map, st.st_size);
		close(fd);
		return (write(2, "Error: Unsupported ELF class or data encoding\n", 45), 1);
	}
	Elf64_Ehdr ehdr = arch_specifics.get_elf_header(map);
	if (ehdr.e_shoff + ehdr.e_shnum * arch_specifics.sizeof_section_header > (unsigned long)st.st_size) {
		munmap(map, st.st_size);
		close(fd);
		return (write(2, "Error: file too short\n", 48), 1);
	}

	unsigned char *shdr = (map + ehdr.e_shoff);
	void *symtab_shdr = NULL;
	void *strtab_shdr = NULL;
	for (uint16_t i = 0; i < ehdr.e_shnum; i++) {
		Elf64_Shdr	section_header = arch_specifics.get_section_header(shdr, i);
		if (section_header.sh_type == SHT_SYMTAB) {
			symtab_shdr = shdr + arch_specifics.sizeof_section_header * i;
		} else if (section_header.sh_type == SHT_STRTAB) {
			if (i != ehdr.e_shstrndx)
				strtab_shdr = shdr + arch_specifics.sizeof_section_header * i;
			else if (strtab_shdr == NULL) {
				// Fallback to first STRTAB if no other found
				strtab_shdr = shdr + arch_specifics.sizeof_section_header * i;
			}
		}
	}
	if (!symtab_shdr || !strtab_shdr) {
		munmap(map, st.st_size);
		close(fd);
		return (write(2, "Error: Cannot find symbol or string table\n", 42), 1);
	}
	Elf64_Shdr symbol_table_header = arch_specifics.get_section_header(symtab_shdr, 0);
	Elf64_Shdr string_table_header = arch_specifics.get_section_header(strtab_shdr, 0);
	if (string_table_header.sh_addr + string_table_header.sh_size > (unsigned long)st.st_size) {
		munmap(map, st.st_size);
		close(fd);
		return (write(2, "Error: Corrupt string table\n", 28), 1);
	}
	if (symbol_table_header.sh_addr + symbol_table_header.sh_size > (unsigned long)st.st_size) {
		munmap(map, st.st_size);
		close(fd);
		return (write(2, "Error: Corrupt symbol table\n", 28), 1);
	}

	void *symtab = map + symbol_table_header.sh_offset;
	char *strtab = (char *)map + string_table_header.sh_offset;
	size_t num_symbols = symbol_table_header.sh_size / arch_specifics.sizeof_symbol;
	t_vector symbols;

	vector_init(&symbols, sizeof(t_symbol));
	for (size_t i = 1; i < num_symbols; i++) {
		t_symbol symbol;
		Elf64_Sym og_symbol = arch_specifics.get_symbol(symtab, i);
		symbol.identifier  = get_identifier(og_symbol, shdr, arch_specifics, ehdr.e_shnum);

		if (should_print_address(symbol.identifier)) {
			fill_addr(symbol.address_str, og_symbol.st_value, arch_specifics.addr_len);
		} else {
			ft_memset(symbol.address_str, ' ', arch_specifics.addr_len);
		}
		symbol.address_str[arch_specifics.addr_len] = '\0';
		symbol.name_offset = og_symbol.st_name;
		symbol.og_index = i;
	
		if (og_symbol.st_name != 0) {
			char *sym_name = strtab + og_symbol.st_name;
			if (sym_name > (strtab + string_table_header.sh_size)) {
				symbol.name = "<corrupt>";
				symbol.name_len = 9;
			} else {
				symbol.name = sym_name;
				symbol.name_len = get_name_len(strtab, og_symbol.st_name, string_table_header.sh_size);
			}
		}
		else {
			if (og_symbol.st_value == 0)
				continue;
			symbol.name = NULL;
		}
		if (is_filtered_symbol(symbol))
			continue;

		size_t insert_index = vector_binary_search(&symbols, &symbol, (int (*)(void *, void*))compare_symbol_names);
		if (vector_addi(&symbols, &symbol, insert_index) == -1) {
			vector_free(&symbols);
			munmap(map, st.st_size);
			return (1);
		}
	}

	vector_foreach(&symbols, (void (*)(void *))print_symbol);

	munmap(map, st.st_size);
	vector_free(&symbols);
	close(fd);
	return 0;
}