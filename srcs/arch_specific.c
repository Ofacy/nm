#include "nm.h"

Elf64_Shdr	get_section_header_x64(void *section_header_array, uint16_t index)
{
	return (((Elf64_Shdr *)section_header_array)[index]);
}

Elf64_Sym	get_symbol_x64(void *symbol_array, size_t index)
{
	return (((Elf64_Sym *)symbol_array)[index]);
}

Elf64_Ehdr	get_elf_header_x64(void *map)
{
	return (*(Elf64_Ehdr*)map);
}

Elf64_Shdr	get_section_header_x32(void *section_header_array, uint16_t index)
{
	Elf64_Shdr converted;
	Elf32_Shdr original;

	original = ((Elf32_Shdr*)section_header_array)[index];
	converted.sh_addr = original.sh_addr;
	converted.sh_addralign = original.sh_addralign;
	converted.sh_entsize = original.sh_entsize;
	converted.sh_flags = original.sh_flags;
	converted.sh_info = original.sh_info;
	converted.sh_link = original.sh_link;
	converted.sh_name = original.sh_name;
	converted.sh_offset = original.sh_offset;
	converted.sh_size = original.sh_size;
	converted.sh_type = original.sh_type;
	return (converted);
}

Elf64_Sym	get_symbol_x32(void *symbol_array, size_t index)
{
	Elf64_Sym converted;
	Elf32_Sym original;

	original = ((Elf32_Sym *)symbol_array)[index];
	converted.st_info = original.st_info;
	converted.st_name = original.st_name;
	converted.st_other = original.st_other;
	converted.st_shndx = original.st_shndx;
	converted.st_size = original.st_size;
	converted.st_value = original.st_value;
	return (converted);
}

Elf64_Ehdr get_elf_header_x32(void *map)
{
	Elf32_Ehdr header = *(Elf32_Ehdr *)map;
	Elf64_Ehdr converted;

	converted.e_type = header.e_type;
	converted.e_machine = header.e_machine;
	converted.e_version = header.e_version;
	converted.e_entry = header.e_entry;
	converted.e_phoff = header.e_phoff;
	converted.e_shoff = header.e_shoff;
	converted.e_flags = header.e_flags;
	converted.e_ehsize = header.e_ehsize;
	converted.e_phentsize = header.e_phentsize;
	converted.e_phnum = header.e_phnum;
	converted.e_shentsize = header.e_shentsize;
	converted.e_shnum = header.e_shnum;
	converted.e_shstrndx = header.e_shstrndx;
	return (converted);
}
