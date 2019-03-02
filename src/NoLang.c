#include <stdlib.h>
#include "Lang.h"

const char * _default_string_table[][2] = {
#include "NoLangStrings.h"
{NULL, NULL}
};

const char *GETSTRING(const char *p)
{
	int i;

	for (i = 0; _default_string_table[i][0]; i++) {
		if (!strcmp(_default_string_table[i][0], p))
			return _default_string_table[i][1];
	}
	/* not found return at least the token */
	return p;
}

