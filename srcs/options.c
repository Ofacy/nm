#include "symbol.h"
#include <libft.h>

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

int compare_symbol_names_reverse(t_symbol *s1, t_symbol *s2)
{
	return -compare_symbol_names(s1, s2);
}

void get_file_options(
	int options,
	int (**is_filtered)(t_symbol),
	int (**compare)(t_symbol *, t_symbol *))
{

	*is_filtered = is_filtered_symbol;
	if (options & FLAG_UNDEFINED)
		*is_filtered = is_defined_symbol;
	else if (options & FLAG_EXTERNAL)
		*is_filtered = is_external_symbol;
	else if (options & FLAG_ALL)
		*is_filtered = is_invalid_symbol;
	*compare = compare_symbol_names;
	if (options & FLAG_NO_SORT)
		*compare = NULL;
	else if (options & FLAG_REVERSE) {
		*compare = compare_symbol_names_reverse;
	}
}
