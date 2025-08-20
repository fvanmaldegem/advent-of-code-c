#pragma once

#include "list.h"

char* str_slice(const char* str, int start, int end);
t_list *str_split(const char* str, const char split_character);