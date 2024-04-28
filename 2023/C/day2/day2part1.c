#include <errno.h>
#include "../basics/basics.h"

#define BUFFER 	200
#define RED 12
#define GREEN 13
#define BLUE 14

int get_color(char *line, char *color, int value)
{
	int round = 0;
	char *address;
	
	address = strstr(line, color);
	for (address; address != NULL && (*address) != '\0'; address = strstr(address, color))
	{
		address--;
		address--;
		//----------------------------------get the value of the specific color 
		round = (*address) - '0';
		address--;
		int multiplier = 10;
		for (address; isdigit(*address) != 0; address--)
		{
			round += ((*address) - '0') * multiplier;
			multiplier *= 10;
		}
		//-----------------------------------------------------------------------
		for (address; (*address) != '\0' && (*address) != ',' && (*address) != ';'; address++){}
		
		if (round > value)
			return (-1);
	}
	return (OK);
}

int verify_game(char *line)
{
	int game = 0;
	int value = 0;

	char *backup;
	char *start;
	backup = strdup(line);
	start = backup;

	while (isdigit(*backup ) == 0)
		backup++;
	// -------------------------------------get the game number
	game = (*backup) - '0';
	backup++;
	while (isdigit(*backup) != 0)
	{
		game = (game * 10) + ((*backup) - '0');
		backup++;
	}
	// ---------------------------------------------------------
	backup = start;
	free(backup);

	// -------------------------------------check if the color value fits the condition
	value = get_color(line, "red", RED);
	if (value == OK)
	{
		value = get_color(line, "green", GREEN);
		if (value == OK)
		{
			value = get_color(line, "blue", BLUE);
			if (value == OK)
				return (game);
		}
	}
	// ---------------------------------------------------------
	return (-1);
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

	for (line; line; line = fgets (buffer, BUFFER, file))
	{
		int game = verify_game(line);
		if (game != -1)
			total += game;
	}

	printf("day 2 part 1: %d\n", total);
	return (OK);
}
