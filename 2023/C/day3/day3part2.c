#include "../basics/basics.h"

#define BUFFER 1000

static FILE *open_file(char *input)
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

static int get_size(char *input, int *col, int *row)
{
	FILE *file;
	file = open_file(input);

	char *line; 
	char buffer[BUFFER];
	line = fgets(buffer, BUFFER, file);

	for (line; line; line = fgets(line, BUFFER, file))
	{
		(*row)++;
		int temp = strlen(line);
		if (temp > (*col))
			(*col) = temp;
	}
	fclose(file);
	return (OK);
}

static char **allocate_memory(char **storage, int row, int col)
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

static int fill_storage(char ***storage, char *input, int col, int row)
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

static int get_number(char **storage, int row, int col)
{
	for (col; storage[row][col + 1] != '\0' && isdigit(storage[row][col + 1]) != 0; col++){} //--move to the end of the number

	int number = storage[row][col] - '0';
	int multiplier = 10;
	col--;
	for (col ; col > -1 && isdigit(storage[row][col]) != 0; col--)
	{
		number += (storage[row][col] - '0') * multiplier;
		multiplier *= 10;
	}
	return (number);
}

static int verify_surroundings(char **storage, int row, int col)
{
	int adjacent = 0;
	int total = 1;

	if ( col - 1 > -1 && isdigit(storage[row][col - 1]) != 0)  // LEFT
	{
		int number = get_number(storage, row, col - 1);  //count the number to the left
		adjacent++;
		total *= number;		
	}

	if (col + 1 != '\0' && isdigit(storage[row][col + 1]) != 0)  // RIGHT
	{
		int number = get_number(storage, row, col + 1);  //count the number to the right
		adjacent++;
		total *= number;		
	}
	//----------------------------------------------------------------------

	if (row - 1 > -1) // ABOVE ROW 
	{
		if (isdigit(storage[row - 1][col]) != 0)
		{
			int number = get_number(storage, row - 1, col);  //count the number to the right
			adjacent++;
			total *= number;	
		}
		else
		{
			if (isdigit(storage[row - 1][col - 1]) != 0)
			{
				int number = get_number(storage, row - 1, col - 1);  //count the number to the right
				adjacent++;
				total *= number;	
			}
			if (isdigit(storage[row - 1][col + 1]) != 0)
			{
				int number = get_number(storage, row - 1, col + 1);  //count the number to the right
				adjacent++;
				total *= number;	
			}
		}		
	}
	//----------------------------------------------------------------------

	if (storage[row + 1] != NULL) // BELOW ROW
	{
		if (isdigit(storage[row + 1][col]) != 0)
		{
			int number = get_number(storage, row + 1, col);  //count the number to the right
			adjacent++;
			total *= number;	
		}
		else
		{
			if (storage[row + 1][col - 1] && isdigit(storage[row + 1][col - 1]) != 0)
			{
				int number = get_number(storage, row + 1, col - 1);  //count the number to the right
				adjacent++;
				total *= number;	
			}
			if (storage[row + 1][col + 1] != '\0' && isdigit(storage[row + 1][col + 1]) != 0)
			{
				int number = get_number(storage, row + 1, col + 1);  //count the number to the right
				adjacent++;
				total *= number;	
			}
		}		
	}
	//----------------------------------------------------------------------
	if (adjacent != 2)
		return (-1);
	return (total);
}

static int run_storage(char **storage)
{
	int total = 0;			

	for (int i = 0; storage[i] != NULL; i++)
	{
		for(int j = 0; storage[i][j] != '\0'; j++)
		{
			if (storage[i][j] == '*')
			{
				int arround = verify_surroundings(storage, i, j); 
				if (arround > 0)
					total += arround;
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
	storage = allocate_memory(storage, col, row);
	fill_storage(&storage, argv[1], col, row);
	
	printf("day 3 part 2: %d\n", run_storage(storage));

	basics_free_double_array(&storage);
	return (OK);
}