#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../lib/helper.h"
#include "../lib/str.h"

// Return -1 if no number could be properly found, otherwise it returns the index of the end of the number
int get_num_end(char *input, const int n_start, const char end_char) {
    for (int i = n_start; i < strlen(input); i++) {
        const char c = input[i];
        if (c == end_char) {
            return i;
        }

        if (isdigit(c)) {
            continue;
        }

        return -1;
    }

    return -1;
}

int solve1(char *input) {
    int total = 0;
    char mul_instruction[5] = "mul(";

    for (int i = 0; i < strlen(input); i++) {
        char *mul_str = str_slice(input, i, i + strlen(mul_instruction));
        if (strcmp(mul_str, mul_instruction) != 0) {
            continue;
        }

        int n1_start = i + strlen(mul_instruction);
        int n1_end = get_num_end(input, n1_start , ',');
        if (n1_end == -1) {
            continue;
        }

        char *str_n1 = str_slice(input, n1_start, n1_end);
        int n1 = atoi(str_n1);

        int n2_start = n1_end + 1;
        int n2_end = get_num_end(input, n2_start , ')');
        if (n2_end == -1) {
            continue;
        }
        char *str_n2 = str_slice(input, n2_start, n2_end);
        int n2 = atoi(str_n2);

        total = total + n1 * n2;
    }

    return total;
}

int solve2(char *input) {
    int total = 0;
    bool mul_enabled = true;
    char mul_instruction[5] = "mul(";
    char do_instruction[5] = "do()";
    char dont_instruction[8] = "don't()";

    for (int i = 0; i < strlen(input); i++) {
        char *do_str = str_slice(input, i, i + strlen(do_instruction));
        if (strcmp(do_str, do_instruction) == 0) {
            mul_enabled = true;
        }

        char *dont_str = str_slice(input, i, i + strlen(dont_instruction));
        if (strcmp(dont_str, dont_instruction) == 0) {
            mul_enabled = false;
        }

        if (!mul_enabled) {
            continue;
        }

        char *mul_str = str_slice(input, i, i + strlen(mul_instruction));
        if (strcmp(mul_str, mul_instruction) != 0) {
            continue;
        }

        int n1_start = i + strlen(mul_instruction);
        int n1_end = get_num_end(input, n1_start , ',');
        if (n1_end == -1) {
            continue;
        }

        char *str_n1 = str_slice(input, n1_start, n1_end);
        int n1 = atoi(str_n1);

        int n2_start = n1_end + 1;
        int n2_end = get_num_end(input, n2_start , ')');
        if (n2_end == -1) {
            continue;
        }
        char *str_n2 = str_slice(input, n2_start, n2_end);
        int n2 = atoi(str_n2);

        total = total + n1 * n2;
    }

    return total;
}

int main() {
    char *input = get_input(2024, 3, 1);
    printf("answer 1: %d\n", solve1(input));

    input = get_input(2024, 3, 2);
    printf("answer 2: %d\n", solve2(input));
    free(input);
}
