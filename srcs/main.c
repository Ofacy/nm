#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <elf.h>
#include <sys/stat.h>
#include <stdio.h>

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

int handle_file(
	const char *filepath,
	int (*is_filtered)(t_symbol),
	int (*compare)(t_symbol *, t_symbol *))
{
	int fd = open(filepath, O_RDONLY);
	if (fd < 0) {
		perror(filepath);
		return 1;
	}

	struct stat st;
	if (fstat(fd, &st) < 0) {
		close(fd);
		perror("fstat");
		return 1;
	}
	unsigned char *map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	if (map == MAP_FAILED) {
		perror("mmap");
		return 1;
	}

	t_arch_functions arch_specifics;
	if (map[EI_MAG0] != ELFMAG0 ||
		map[EI_MAG1] != ELFMAG1 ||
		map[EI_MAG2] != ELFMAG2 ||
		map[EI_MAG3] != ELFMAG3) {
		munmap(map, st.st_size);
		write(2, "Error: Not an ELF file\n", 23);
		return 1;
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
		write(2, "Error: Unknown ELF class\n", 25);
		return 1;
	}
	Elf64_Ehdr ehdr = arch_specifics.get_elf_header(map);
	if (ehdr.e_shoff + ehdr.e_shnum * arch_specifics.sizeof_section_header > (unsigned long)st.st_size) {
		munmap(map, st.st_size);
		write(2, "Error: Corrupt section headers\n", 31);
		return 1;
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
		write(2, "Error: Missing symbol or string table\n", 38);
		return 1;
	}
	Elf64_Shdr symbol_table_header = arch_specifics.get_section_header(symtab_shdr, 0);
	Elf64_Shdr string_table_header = arch_specifics.get_section_header(strtab_shdr, 0);
	if (string_table_header.sh_addr + string_table_header.sh_size > (unsigned long)st.st_size) {
		munmap(map, st.st_size);
		write(2, "Error: Corrupt string table\n", 28);
		return 1;
	}
	if (symbol_table_header.sh_addr + symbol_table_header.sh_size > (unsigned long)st.st_size) {
		munmap(map, st.st_size);
		write(2, "Error: Corrupt symbol table\n", 28);
		return 1;
	}

	void *symtab = map + symbol_table_header.sh_offset;
	char *strtab = (char *)map + string_table_header.sh_offset;
	size_t num_symbols = symbol_table_header.sh_size / arch_specifics.sizeof_symbol;
	t_vector symbols;

	vector_init(&symbols, sizeof(t_symbol));
	for (size_t i = 0; i < num_symbols; i++) {
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
			symbol.name_offset = 0;
			symbol.name = NULL;
			symbol.name_len = 0;
			if (og_symbol.st_shndx != SHN_UNDEF && og_symbol.st_shndx < ehdr.e_shnum && og_symbol.st_value == 0) {
				Elf64_Shdr sec = arch_specifics.get_section_header(shdr, og_symbol.st_shndx);
				symbol.name_offset = sec.sh_name;
				symbol.name = strtab + sec.sh_name;
				symbol.name_len = get_name_len(strtab, sec.sh_name, string_table_header.sh_size);
			}
			if (is_filtered != is_invalid_symbol && og_symbol.st_value == 0)
				continue;
		}
		if (is_filtered && is_filtered(symbol))
			continue;

		size_t insert_index = symbols.len;
		if (compare) {
			insert_index = vector_binary_search(&symbols, &symbol, (int (*)(void *, void *))compare);
		}
		if (vector_addi(&symbols, &symbol, insert_index) == -1) {
			vector_free(&symbols);
			munmap(map, st.st_size);
			perror("vector_addi");
			return (1);
		}
	}

	vector_foreach(&symbols, (void (*)(void *))print_symbol);

	munmap(map, st.st_size);
	vector_free(&symbols);
	return 0;
}

int main(int argc, char **argv)
{
	int options;
	int errors;
	t_vector file_names;
	int (*is_filtered)(t_symbol);
	int (*compare)(t_symbol *, t_symbol *);

	errors = 0;
	options = 0;
	vector_init(&file_names, sizeof(char *));
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			for (int j = 1; argv[i][j] != '\0'; j++) {
				if (argv[i][j] == 'a')
					options |= FLAG_ALL;
				else if (argv[i][j] == 'u')
					options |= FLAG_UNDEFINED;
				else if (argv[i][j] == 'g')
					options |= FLAG_EXTERNAL;
				else if (argv[i][j] == 'p')
					options |= FLAG_NO_SORT;
				else if (argv[i][j] == 'r')
					options |= FLAG_REVERSE;
				else {
					ft_printf("%s: invalid option -- '%c'\n", argv[0], argv[i][j]);
					ft_printf("Usage: %s [-agunpr] [file ...]\n", argv[0]);
					return (1);
				}
			}
		}
		else {
			if (vector_add(&file_names, &argv[i]) == -1) {
				perror("vector_add");
				return (1);
			}
		}
	}
	get_file_options(options, &is_filtered, &compare);
	if (file_names.len == 0) {
		if (handle_file("a.out", is_filtered, compare) != 0)
			errors++;
	}
	else {
		for (size_t i = 0; i < file_names.len; i++) {
			char *filepath = *((char **)vector_get(&file_names, i));
			if (file_names.len > 1)
				ft_printf("\n%s:\n", filepath);
			if (handle_file(filepath, is_filtered, compare) != 0)
				errors++;
		}
	}
	vector_free(&file_names);
	return errors;
}
