#include "../basics/basics.h"

#define BUFFER 1024
#define DATA 3

int get_seeds(char *file_name)
{
	FILE *file = fopen(file_name, "r");
	if (file == NULL)
		basics_exit_error("no file! go away!", KO);
	
	char buffer[BUFFER];
	char *line;
	line = fgets(buffer, BUFFER, file);
	int size = 0;
	char *temp = strstr(line, " ");
	for (int i = 0; temp[i] != '\0'; i++)
	{
		if (isdigit(temp[i]) != 0 && (temp[i + 1] == '\n' || temp[i + 1] == ' '))
			size++;
	}
	fclose(file);
	return (size);
}

double make_double(char *str)
{
	double number;
	int size = strlen(str) - 1;
	if (str[size] == '\n')
		size--;
	number = str[size] - '0';

	double multiplier = 10;
	for (int i = size - 1; i > -1 && isdigit(str[i]) != 0; i--)
	{
		number = ((str[i] - '0') * multiplier) + number;
		multiplier *= 10;
	}
	return (number);
}

void fill_storage(char *line, double **storage, int size)
{
	line = strstr(line, " ");
	char *dup = strdup(line);
	char *backup;
	backup = dup;
	dup++;

	char *temp;
	for (int i = 0; (temp = strsep(&dup, " ")) != NULL || (temp = strsep(&dup, "\n")) != NULL; i++)
		(*storage)[i] = make_double(temp);
	free(backup);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("aren't you forgeting something?\n");
		return (KO);
	}
	
	int nr_seeds = get_seeds(argv[1]);

	FILE *file = fopen(argv[1], "r");
	if (file == NULL)
		basics_exit_error("no file! go away!", KO);

	char buffer[BUFFER];
	char *line;
	line = fgets(buffer, BUFFER, file);

	// create the storage array
	double *seeds = malloc(sizeof(double) * nr_seeds);
	if (!seeds)
		basics_exit_error("malloc failure", MFAIL);
	fill_storage(line, &seeds, nr_seeds);
	line = fgets(buffer, BUFFER, file);

	for (line; line; line = fgets(buffer, BUFFER, file))
	{
		int tracker[nr_seeds];
		for (int i = 0; i < nr_seeds; i++)
			tracker[i] = 0;
		while (line[0] != '\n')
		{
			if (isdigit(line[0]) == 0)
				line = fgets(buffer, BUFFER, file);
			
			double array[DATA];
			char *temp;
			for (int i = 0; (temp = strsep(&line, " ")) != NULL || (temp = strsep(&line, "\n")) != NULL; i++)
				array[i] = basics_string_to_double(temp);
			
			for (int i = 0; i < nr_seeds; i++)
			{
				if (seeds[i] >= array[1] && seeds[i] < (array[1] + array[2]))
				{
					if (tracker[i] == 0)
					{
						seeds[i] = array[0] + (seeds[i] - array[1]);
						tracker[i] = 1;
					}
				}
			}
			line = fgets(buffer, BUFFER, file);
			if (!line)
				break ;
		}
	}
	fclose(file);

	double smallest = seeds[0];
	for (int i = 1; i < nr_seeds; i++)
	{
		if (smallest > seeds[i])
			smallest = seeds[i];
	}
	free(seeds);
	printf("day 5 part 1: %0.f\n", smallest);
	return (0);
}