#include <strings.h>
#include <math.h>
#include "../basics/basics.h"

#define BUFFER 200
#define SIZE 25
#define CARDS 201

// #define SIZE 8
// #define CARDS 6

static char **create_array(int size)
{
	char **array;
	array = malloc(sizeof(char*) * size);
	if (!array)
		basics_exit_error("malloc failure", MFAIL);
	array[size - 1] = NULL;

	for (int i = 0; i < size - 1; i++)
	{
		array[i] = malloc(sizeof(char) * 3);
		if (!array[i])
		{
			for (i ; i > -1; i--)
				free(array[i]);
			free(array);
			array = NULL;
			basics_exit_error("malloc failure", MFAIL);
		}
		bzero(array[i], 3);
	}
	return (array);
}

static int **create_int_array(int size)
{
	int **array;
	array = malloc(sizeof(int*) * size);
	if (!array)
		basics_exit_error("malloc failure", MFAIL);
	array[size - 1] = NULL;

	for (int i = 0; i < size - 1; i++)
	{
		array[i] = malloc(sizeof(int));
		if (!array[i])
		{
			for (i ; i > -1; i--)
				free(array[i]);
			free(array);
			array = NULL;
			basics_exit_error("malloc failure", MFAIL);
		}
	}
	return (array);
}

static double get_point(char **win, char *temp)
{
	for (int i = 0; i < SIZE; i++)
	{
		if (strcmp(win[i], temp) == 0)
			return (1);
	}
	return (0);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("aren't you forgeting something?\n");
		return (OK);
	}
	
	FILE *file = fopen(argv[1], "r");
	if (file == NULL)
		basics_exit_error("no file! go away!\n", OK);

	char buffer[BUFFER];
	char *line;
	line = fgets(buffer, BUFFER, file);

	int games[CARDS + 1];
	games[CARDS] = '\0';
	int games_i = 0;

	for (line; line; line = fgets(buffer, BUFFER, file))
	{
		char *temp = strstr(line, ":");
		char *temp_win = strstr(line, "|");
		char **win = create_array(SIZE + 1); // malloc protected inside the function
		int win_i = 0;

		// read and store the winning numbers 
		for (int i = 0; temp_win[i] != '\0'; i++)
		{
			if (isdigit(temp_win[i]) != 0)
			{
				int number = temp_win[i] - '0';
				i++;
				for (i; isdigit(temp_win[i]) != 0; i++)
					number = (number * 10) + (temp_win[i] - '0');
				i--;
				sprintf(win[win_i], "%d", number); // equivalent of itoa
				win_i++;
			}
		}

		// go through the drawn numbers and store them 
		int line_score = 0;
		for (int i = 0; temp[i] != '|'; i++)
		{
			if (isdigit(temp[i]) != 0)
			{
				int number = temp[i] - '0';
				i++;
				for (i; isdigit(temp[i]) != 0; i++)
					number = (number * 10) + (temp[i] - '0');
				i--;
				char *draw = malloc(sizeof(char) * 3);
				if (!draw)
					basics_exit_error("malloc failure\n", MFAIL);
				bzero(draw, 3);

				sprintf(draw, "%d", number); // equivalent of itoa
				line_score += (int)get_point(win, draw); // see if it was a winning number 
				free(draw);
			}
		}
		basics_free_double_array(&win);
		games[games_i] = line_score;
		games_i++;
	}

	int instances[CARDS + 1];
	for (int i = 0; i < CARDS; i++)
		instances[i] = 1; 
	instances[CARDS] = '\0';

	int total = 0;
	for (int i = 0; i < CARDS; i++)
	{
		for (int j = 0; j < instances[i]; j++)
		{
			for (int k = 0; k < games[i]; k++)
				instances[(i + 1) + k]++;
		}
		total += instances[i];
	}
	printf("day 4 part 2: %d\n", total);
	return (OK);
}