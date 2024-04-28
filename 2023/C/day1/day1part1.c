#include <fcntl.h>
#include <errno.h>
#include "../basics/basics.h"

#define BUFFER 50

int get_row_number(char *line)
{
	int first = 0;
	int second = 0;
	int first_set = 0;
	int second_set = 0;

	for (int i = 0; line[i] != '\0'; i++)
	{
		if (isdigit(line[i]) != 0)
		{
			if (first_set == 0)
			{
				first = line[i] - '0';
				first_set = 1;
			}
			else
			{
				second = line[i] - '0';
				second_set = 1;
			}
		}
	}
	if (first_set == 0)
		return (0);
	if (second_set == 0)
		second = first;
	int row_number = (first * 10) + second;

	return (row_number);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("aren't you forgeting something?\n");
		return (OK);
	}

	FILE *file;
	file = fopen(argv[1], "r");
	if (file == NULL)
	{
		printf("no file! go away!\n");
		return (errno);
	}

	char *line;
	char buffer[BUFFER];
	line = fgets (buffer, BUFFER, file);
	int total = 0;

	for (line; line; line = fgets(buffer, BUFFER, file))
	{
		int row_number = get_row_number(line);
		total += row_number;
	}
	fclose(file);

	printf("day 1 part 1: %d\n", total);
	return (OK);
}