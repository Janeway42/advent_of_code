#include "basics.h"

static int	count_str(char const *s, char c)
{
	int counter = 0;

	for (int i = 0; s[i] != '\0'; i++)
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			counter++;
	}
	return (counter);
}

static int	str_length(char const *str, char c)
{
	int size = 0;

	for (int i = 0; str[i] == c; i++){}

	for (int i = 0; str[i] != '\0' && str[i] != c; i++)
		size++;
	return (size);
}

static void	*cleanup_memory(char **output, int position)
{
	for (int i = 0; i < position; i++)
		free(output[i]);
	free(output);
	return (NULL);
}

static char	**fill_output(char const *str, int amount_str, char c, char **output)
{
	int i = -1;
	for (; i < amount_str;)
	{
		++i;
		if (i == amount_str)
			break ;
		while(*str == c)
			str++;
		int size = str_length(str, c);
		output[i] = malloc(sizeof(char) * (size + 1));
		if (!output[i])
		{
			printf("malloc failure\n");
			return (cleanup_memory(output, i));
		}
		int j = 0;
		for (; j < size; j++)
		{
			output[i][j] = *str;
			str++;
		}
		output[i][j] = '\0';
	}
	output[i] = NULL;
	return (output);
}

char** basics_split(char* const str, char c)
{
	char** output;
	int amount_str = 0;

	if (!str)
	{
		printf("invalid input\n");
		return (NULL);
	}

	amount_str = count_str(str, c);
	output = malloc(sizeof(*output) * (amount_str + 1));
	if (!output)
	{
		printf("malloc failure\n");
		return (NULL);
	}
	output = fill_output(str, amount_str, c, output);
	return (output);
}

/*
** int		main(void)
** {
** 	char	tosplit[] = "a1234a5678a90";
** 	char	**pointers_arr;
** 	int		i;
** 	char	c;
**
** 	i = 0;
** 	c = 'a';
** 	pointers_arr = ft_split(tosplit, c);
** 	while (i != ft_counter_arrays(tosplit, c))
** 	{
** 		printf("%s\n", (char *)pointers_arr[i]);
** 		i++;
** 	}
** 	return (0);
** }
*/
