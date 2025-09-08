#pragma once

#include "list.h"

char* get_input(int year, int day);

/* Parse a single string with columns to multiple list.

Parameters
char *text: the text with columns to parse
int columns: the amount of columns

the remaing parameters are of the type 't_list*' and will store the values of the columns.
*/
void parse_int_columns(char *text, int columns, ...);
t_list *parse_int_matrix(char *text);