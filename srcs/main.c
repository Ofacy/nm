#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "elf.h"
#include <sys/stat.h>

#include <libft.h>

char get_identifier(Elf64_Sym sym, Elf64_Shdr *shdr, size_t shnum)
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
				Elf64_Shdr sec = shdr[sym.st_shndx];
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

int is_filtered_symbol(char identifier)
{
	return (identifier == '?' || identifier == 'n' || identifier == 'N' || identifier == 'a');
}

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
	const char *strtab = (char *)map + strtab_shdr->sh_offset;
	size_t num_symbols = symtab_shdr->sh_size / sizeof(Elf64_Sym);
	for (size_t i = 1; i < num_symbols; i++) {
		char identifier = get_identifier(symtab[i], shdr, ehdr->e_shnum);
		char addr_str[17];

		if (is_filtered_symbol(identifier))
			continue;
		if (should_print_address(identifier)) {
			fill_addr(addr_str, symtab[i].st_value);
		} else {
			ft_memset(addr_str, ' ', 16);
		}
		write(1, addr_str, 16);
		write(1, " ", 1);
		write(1, &identifier, 1);
		write(1, " ", 1);
		if (symtab[i].st_name != 0) {
			const char *sym_name = strtab + symtab[i].st_name;
			if (sym_name > (strtab + strtab_shdr->sh_size)) {
				write(1, "<corrupt>", 9);
			} else
				write(1, sym_name, ft_strlen(sym_name));
		}
		write(1, "\n", 1);
	}

	munmap(map, st.st_size);
	close(fd);
	return 0;
}