#include "basics.h"

int basics_exit_error(char *str, int error)
{
	printf("%s\n", str);
	exit(error);
}
