#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <elf.h>
#include <sys/stat.h>

#include <libft.h>
#include <vector.h>

#include "symbol.h"

void fill_addr(char *dest, Elf64_Addr addr)
{
	for (int i = 15; i >= 0; i--) {
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
	void *map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (map == MAP_FAILED) {
		close(fd);
		return (write(2, "Error: Cannot map file\n", 23), 1);
	}

	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)map;
	if (ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
		ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
		ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
		ehdr->e_ident[EI_MAG3] != ELFMAG3) {
		munmap(map, st.st_size);
		close(fd);
		return (write(2, "Error: Not a valid ELF file\n", 28), 1);
	}
	if (ehdr->e_ident[EI_CLASS] != ELFCLASS64) {
		munmap(map, st.st_size);
		close(fd);
		return (write(2, "Error: Unsupported ELF class or data encoding\n", 45), 1);
	}

	Elf64_Shdr *shdr = (Elf64_Shdr *)((char *)map + ehdr->e_shoff);
	Elf64_Shdr *symtab_shdr = NULL;
	Elf64_Shdr *strtab_shdr = NULL;
	for (int i = 0; i < ehdr->e_shnum; i++) {
		if (shdr[i].sh_type == SHT_SYMTAB) {
			symtab_shdr = &shdr[i];
		} else if (shdr[i].sh_type == SHT_STRTAB) {
			if (i != ehdr->e_shstrndx)
				strtab_shdr = &shdr[i];
			else if (strtab_shdr == NULL) {
				// Fallback to first STRTAB if no other found
				strtab_shdr = &shdr[i];
			}
		}
	}
	if (!symtab_shdr || !strtab_shdr) {
		munmap(map, st.st_size);
		close(fd);
		return (write(2, "Error: Cannot find symbol or string table\n", 42), 1);
	}
	if (strtab_shdr->sh_addr + strtab_shdr->sh_size > (unsigned long)st.st_size) {
		munmap(map, st.st_size);
		close(fd);
		return (write(2, "Error: Corrupt string table\n", 28), 1);
	}
	if (symtab_shdr->sh_addr + symtab_shdr->sh_size > (unsigned long)st.st_size) {
		munmap(map, st.st_size);
		close(fd);
		return (write(2, "Error: Corrupt symbol table\n", 28), 1);
	}

	Elf64_Sym *symtab = (Elf64_Sym *)((char *)map + symtab_shdr->sh_offset);
	char *strtab = (char *)map + strtab_shdr->sh_offset;
	size_t num_symbols = symtab_shdr->sh_size / sizeof(Elf64_Sym);
	t_vector symbols;

	vector_init(&symbols, sizeof(t_symbol));
	for (size_t i = 1; i < num_symbols; i++) {
		t_symbol symbol;
		symbol.identifier  = get_identifier(symtab[i], shdr, ehdr->e_shnum);

		if (is_filtered_symbol(symbol.identifier))
			continue;
		if (should_print_address(symbol.identifier)) {
			fill_addr(symbol.address_str, symtab[i].st_value);
		} else {
			ft_memset(symbol.address_str, ' ', 16);
		}
		symbol.address_str[16] = '\0';
		symbol.name_offset = symtab[i].st_name;
		symbol.og_index = i;
		if (symtab[i].st_name != 0) {
			char *sym_name = strtab + symtab[i].st_name;
			if (sym_name > (strtab + strtab_shdr->sh_size)) {
				symbol.name = "<corrupt>";
				symbol.name_len = 9;
			} else {
				symbol.name = sym_name;
				symbol.name_len = get_name_len(strtab, symtab[i].st_name, strtab_shdr->sh_size);
			}
		}
		else {
			if (symtab[i].st_value == 0)
				continue;
		}

		size_t insert_index = vector_binary_search(&symbols, &symbol, (int (*)(void *, void*))compare_symbol_names);
		if (vector_addi(&symbols, &symbol, insert_index) == -1) {
			vector_free(&symbols);
			munmap(map, st.st_size);
			return (1);
		}
	}

	vector_foreach(&symbols, (void (*)(void *))print_symbol);

	vector_free(&symbols);
	munmap(map, st.st_size);
	close(fd);
	return 0;
}