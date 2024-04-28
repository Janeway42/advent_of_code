#include <fcntl.h>
#include <errno.h>
#include "../basics/basics.h"

#define BUFFER 100

int verify_numbers(char *str)
{
	char *array[10] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
	char *real = "123456789";
	char *address;

	for (int i = 0; i < 9; i++)
	{
		address = strstr(str, array[i]);
		if (address != NULL)
			return (real[i] - '0');
	}
	return (-1);
}

int get_row_number(char *line)
{
	int first = -1;
	int last = -1;
	int size = strlen(line) - 1;

	//verify from front 
	int count = 0;
	while (count < size)
	{
		if (isdigit(line[count]) != 0)
			first = line[count] - '0';
		else
		{
			char *temp;
			temp = malloc(sizeof(char) * (count + 2));
			if (!temp)
				basics_exit_error("malloc failure", MFAIL);
			temp[count + 1] = '\0';
			temp = strncpy(temp, line, count + 1);
			int number = verify_numbers(temp);
			if (number != -1) 
				first = number; 
			free(temp);
		}
		if (first != -1)
			break ;
		count++;
	}

	//verify from back 
	count = strlen(line) - 1;
	while (count > -1)
	{
		if (isdigit(line[count]) != 0)
			last = line[count] - '0';
		else
		{
			char *backup = strdup(line);
			char *start = backup;
			for (int i = 0; i < count; i++)
				backup++;
			int number1 = verify_numbers(backup);
			if (number1 != -1)
				last = number1;
			backup = start;
			free(backup);
		}
		if (last != -1)
			break ;
		count--;
	}

	int row_number = (first * 10) + last;
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
		if (row_number == -1)
			basics_exit_error("error number", KO);
		total += row_number;
	}
	fclose(file);	

	printf("day 1 part 2: %d\n", total);
	return (OK);
}
