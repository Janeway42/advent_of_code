#include "../basics/basics.h"

#define BUFFER 1024

static void fill_data(double *time, double *distance, FILE *file)
{
	char *line;
	char buffer[BUFFER];
	line = fgets(buffer, BUFFER, file);

	for (line; line; line = fgets(buffer, BUFFER, file))
	{
		int size = strlen(line);
		double number = 0;
		int i = 0;
		while (i < size)
		{
			for (i; isdigit(line[i]) == 0; i++){}
			for (i; isdigit(line[i]) != 0 && i < size; i++)
				number = (number * 10) + (line[i] - '0');
		}
		if (line[0] == 'T')
			*time = number;
		else if (line[0] == 'D')
			*distance = number;
	}
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

	// storage
	double time;
	double distance;
	fill_data(&time, &distance, file);

	// run calculations 
	int total = 0;
	int half = time / 2;
	for (double i = half; i > -1; i--)
	{
		if (i * (time - i) <= distance)
			break ;
		else
			total++;
	}

	// modulo 2 for double 
	while (time >= 2)
        time = time - 2;

	if (time == 0)
		total = total + (total - 1);
	else
		total = total * 2;

	printf("day 6 part 2: %d\n", total);
	return (OK);
}