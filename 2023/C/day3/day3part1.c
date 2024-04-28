#include "../basics/basics.h"

#define BUFFER 1000

FILE *open_file(char *input)
{
	FILE *file;
	file = fopen(input, "r");
	if (file == NULL)
	{
		printf("no file! go away!\n");
		exit(KO);
	}
	return (file);
}

int get_size(char *input, int *col, int *row)
{
	FILE *file;
	file = open_file(input);

	char *line; 
	char buffer[BUFFER];
	line = fgets(buffer, BUFFER, file);

	for (line; line; line = fgets(buffer, BUFFER, file))
	{
		(*row)++;
		int temp = 0;
		temp = strlen(line);
		if (temp > *col)
			(*col) = temp;
	}
	fclose(file);
	return (OK);
}

char **assign_memory(char **storage, int row, int col)
{
	storage = malloc(sizeof(char*) * (row + 1));
	if (!storage)
		basics_exit_error("malloc failure", MFAIL);
	storage[row] = NULL;

	for (int i = 0; i < row; i++)
	{
		storage[i] = malloc(sizeof(char *) * col);
		if (!storage[i])
		{
			for (int k = i; k > -1; k--)
				free(storage[i]);
			free(storage);
			basics_exit_error("malloc failure", MFAIL);
		}
		storage[i][col] = '\0';
	}
	return (storage);
}

int fill_storage(char ***storage, char *input, int col, int row)
{
	FILE *file;
	file = open_file(input);

	char *line; 
	char buffer[BUFFER];
	line = fgets(buffer, BUFFER, file);

	while (line)
	{
		for (int row_count = 0; row_count < row; row_count++)
		{
			int i = 0;
			for (i; i < col - 1; i++)
				(*storage)[row_count][i] = line[i];
			(*storage)[row_count][i] = '\0';
			line = fgets(buffer, BUFFER, file);
		}
	}
	fclose(file);
	return (OK);
}

int element(char element)
{
	if (element != '.' && isdigit(element) == 0)
		return (1);
	return (OK);
}

int verify_surroundings(char **storage, int row, int col, int row_size)
{
	// top row
	if (row - 1 == -1)
	{
		if (col - 1 == -1 )
		{
			if (element(storage[row][col + 1]) == 1 || element(storage[row + 1][col]) == 1 || element(storage[row + 1][col - 1]) == 1)
				return (1);
		}
		else if (storage[row][col + 1] == '\0')
		{
			if (element(storage[row][col - 1]) == 1 || element(storage[row + 1][col]) == 1 || element(storage[row + 1][col - 1]) == 1)
				return (1); 
		}
		else 
		{
			if (element(storage[row][col - 1]) == 1 || element(storage[row][col + 1]) == 1
				|| element(storage[row + 1][col - 1]) == 1 || element(storage[row + 1][col]) == 1 || element(storage[row + 1][col + 1]) == 1)
					return (1);
		}
	}

	// bottom row
	else if (row + 1 == row_size)
	{
		if (element(storage[row][col + 1]) == 1 || element(storage[row - 1][col]) == 1 || element(storage[row - 1][col - 1]) == 1)
				return (1);
		else if (storage[row][col + 1] == '\0')
		{
			if (element(storage[row][col - 1]) == 1 || element(storage[row - 1][col]) == 1 || element(storage[row - 1][col - 1]) == 1)
				return (1); 
		}
		else 
		{
			if (element(storage[row][col - 1]) == 1 || element(storage[row][col + 1]) == 1
				|| element(storage[row - 1][col - 1]) == 1 || element(storage[row - 1][col]) == 1 || element(storage[row - 1][col + 1]) == 1)
				return (1);
		}
	}

	// middle row
	else 
	{
		if (col - 1 == -1)
		{
			if (element(storage[row][col + 1]) == 1 
				|| element(storage[row - 1][col]) == 1 || element(storage[row - 1][col + 1]) == 1
				|| element(storage[row + 1][col]) == 1 || element(storage[row + 1][col + 1]) == 1)
				return (1);
		}
		else if (storage[row][col + 1] == '\0')
		{
			if (element(storage[row][col - 1]) == 1 
				|| element(storage[row - 1][col]) == 1 || element(storage[row - 1][col - 1]) == 1
				|| element(storage[row + 1][col]) == 1 || element(storage[row + 1][col - 1]) == 1)
				return (1);
		}
		else
		{
			if (element(storage[row][col - 1]) == 1 || element(storage[row][col + 1]) == 1  
				|| element(storage[row - 1][col - 1]) == 1 || element(storage[row - 1][col]) == 1 || element(storage[row - 1][col + 1]) == 1
				|| element(storage[row + 1][col - 1]) == 1 || element(storage[row + 1][col]) == 1 || element(storage[row + 1][col + 1]) == 1)
					return (1);
		}

	}
	return (OK); // means it has no adjason character 
}

int run_storage(char **storage, int row_size)
{
	int total = 0;
	int row = 0;

	for (row; storage[row] != NULL; row++)
	{
		for(int i = 0; storage[row][i] != '\0'; i++)
		{
			if (isdigit(storage[row][i]) != 0)
			{
				int number = storage[row][i] - '0';
				int arround = verify_surroundings(storage, row, i, row_size); 
				i++;
				for (i; storage[row][i] != '\0' && isdigit(storage[row][i]) != 0; i++)
				{
					if (arround == 0)
						arround = verify_surroundings(storage, row, i, row_size); 
					number = (number * 10) + (storage[row][i] - '0');
				}
				if (arround == 1)
					total += number;
				i--;
			}
		}
	}
	return (total);
}

int main (int argc, char **argv)
{
	if (argc != 2)
	{
		printf("aren't you forgeting something?\n");
		return (OK);
	}

	int col = 0;
	int row = 0;
	get_size(argv[1], &col, &row); // col has \n

	char **storage;
	storage = assign_memory(storage, col, row);
	fill_storage(&storage, argv[1], col, row);
	
	printf("day 3 part 1: %d\n", run_storage(storage, row));

	basics_free_double_array(&storage);
	return (OK);
}