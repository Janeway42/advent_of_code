#include "basics.h"

void basics_free_double_array(char ***array)
{
	for (int i = 0; (*array)[i] != NULL; i++)
		free((*array)[i]);
	free((*array));
	(*array) = NULL;
}