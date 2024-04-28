#include "basics.h"

char **basics_allocate_memory_double(char **storage, int row, int col)
{
	storage = malloc(sizeof(char*) * (row + 1));
	if (!storage)
	{
		printf("malloc failure\n");
		return (NULL);
	}
	storage[row] = NULL;

	for (int i = 0; i < row; i++)
	{
		storage[i] = malloc(sizeof(char ) * (col + 1));
		if (!storage[i])
		{
			printf("malloc failure\n");
			for (int k = i; k > -1; k--)
				free(storage[i]);
			free(storage);
			storage = NULL;
			return (NULL);
		}
		bzero(storage[i], col + 1);
		// storage[i][col] = '\0';
	}
	return (storage);
}
