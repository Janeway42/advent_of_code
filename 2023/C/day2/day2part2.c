#include <errno.h>
#include "../basics/basics.h"

#define BUFFER 	200
#define RED 12
#define GREEN 13
#define BLUE 14

int get_color(char *line, char *color)
{
	int max = 0;
	char *address;
	
	address = strstr(line, color);
	for (address; address != NULL && (*address) != '\0'; address = strstr(address, color))
	{
		address--;
		address--;
		//--------------------------get the value of the specific color 
		int round = (*address) - '0';
		address--;
		int multiplier = 10;
		for (address; isdigit(*address) != 0; address--)
		{
			round += ((*address) - '0') * multiplier;
			multiplier *= 10;
		}
		//--------------------------------------------------------------
		for (address; (*address) != '\0' && (*address) != ',' && (*address) != ';'; address++){}

		if (round > max)
			max = round;
	}
	return (max);
}

int verify_game(char *line)
{
	int game = get_color(line, "red");
	game *= get_color(line, "green");
	game *= get_color(line, "blue");

	return (game);
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
		total += game;		
	}
	printf("day 2 part 2: %d\n", total);
	return (OK);
}
