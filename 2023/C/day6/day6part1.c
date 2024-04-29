#include "../basics/basics.h"

#define BUFFER 1024

static int make_int(char *str)
{
	int number;
	int size = strlen(str) - 1;
	if (str[size] == '\n')
		size--;
	number = str[size] - '0';

	int multiplier = 10;
	for (int i = size - 1; i > -1 && isdigit(str[i]) != 0; i--)
	{
		number = ((str[i] - '0') * multiplier) + number;
		multiplier *= 10;
	}
	return (number);
}

static int get_size(char *line)
{
	int size = 0;
	char *temp = strstr(line, " ");

	for (int i = 0; temp[i] != '\0'; i++)
	{
		if (isdigit(temp[i]) != 0 && (temp[i + 1] == '\n' || temp[i + 1] == ' '))
			size++;
	}
	return (size);
}

static void fill_data(int **time, int **distance, FILE *file, int size)
{
	char buffer[BUFFER];
	char *line;

	line = fgets(buffer, BUFFER, file);
	char **split = basics_split(line , ' ');

	for (int i = 0; i < size; i++)
		(*time)[i] = make_int(split[i + 1]);
	basics_free_double_array(&split);

	line = fgets(buffer, BUFFER, file);
	split = basics_split(line, ' ');
	for (int i = 0; i < size; i++)
		(*distance)[i] = make_int(split[i + 1]);
	basics_free_double_array(&split);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("aren't you forgeting something?\n");
		return (KO);
	}

	FILE *file = fopen(argv[1], "r");
	if (file == NULL)
		basics_exit_error("no file! go away!", KO);
	char buffer[BUFFER];
	char *line;
	line = fgets(buffer, BUFFER, file);	

	int size = get_size(line);
	rewind(file);

	// storage & memory allocation 
	int *time;
	time = malloc(sizeof(int) * size);
	if (!time)
		basics_exit_error("malloc failure", MFAIL);
	int *distance;
	distance = malloc(sizeof(int) * size);
	if (!distance)
		basics_exit_error("malloc failure", MFAIL);
	fill_data(&time, &distance, file, size);
	fclose(file);

	// run calculations 
	int total = 1;

	for (int i = 0; i < size; i++)
	{
		int total_temp = 0;
		int half = time[i] / 2;

		for (int j = half; j > -1; j--)
		{
			if (j * (time[i] - j) <= distance[i])
				break ;
			else
				total_temp++;
		}

		if (time[i] % 2 == 0)
			total = total * (total_temp + (total_temp - 1));
		else
			total = total * (total_temp * 2);
	}

	free(time);
	free(distance);

	printf("day 6 part 1: %d\n", total);
	return (OK);
}