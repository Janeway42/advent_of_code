#include "basics.h"

char *basics_integer_to_string(int number)
{
	long long a = number;
	int size = 0;
	while (a > 0)
	{
		a = a / 10;
		size++;
	}

	char *output;
	output = malloc(sizeof(char) * size);
	if (!output)
	{
		printf("malloc failure\n");
		return (NULL);
	}
	output[size] = '\0';

	int remainder = 0;
	size--;
	while (number > 0)
	{
		remainder = number % 10;
		number = number / 10;
		output[size] = remainder - '0';
		size--;
	}
	return (output);
}

int basics_string_to_number(const char *string)
{
	size_t size = strlen(string) - 1;
	int number = 0;
	int multiplier = 1;

	for (int i = size; i > -1; i--)
	{
		number+= (string[i] - '0') * multiplier;
		multiplier *= 10;
	}

	return (number);
}

double basics_string_to_double(const char *str)
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