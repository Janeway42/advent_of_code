#ifndef BASICS_H
# define BASICS_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define OK 0
#define KO 1
#define MFAIL -1  // malloc fail

// ------------------------------------- edit

char** basics_split(char* const str, char c);

char* basics_integer_to_string(int number);

int basics_string_to_number(const char *string);
double basics_string_to_double(const char *str);

// ------------------------------------- memory

char **basics_allocate_memory_double(char **storage, int row, int col);

// ------------------------------------- free

void basics_free_double_array(char ***array);

// ------------------------------------- error

int basics_exit_error(char *str, int error);

#endif