#include "basics.h"

void basics_print_double_array(char **storage)
{
	for (int row = 0; storage[row] != NULL; row++)
		printf("%s\n", storage[row]);
}