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

    for (int i = 0; i < strlen(input); i++) {
        if (!str_lookahead(input, i, "mul(")) {
            continue;
        }

        int n1_start = i + strlen("mul(");
        int n1_end = get_num_end(input, n1_start , ',');
        if (n1_end == -1) {
            continue;
        }

        char *str_n1 = str_slice(input, n1_start, n1_end);
        int n1 = atoi(str_n1);
        free(str_n1);

        int n2_start = n1_end + 1;
        int n2_end = get_num_end(input, n2_start , ')');
        if (n2_end == -1) {
            continue;
        }
        char *str_n2 = str_slice(input, n2_start, n2_end);
        int n2 = atoi(str_n2);
        free(str_n2);


        total = total + n1 * n2;
    }

    return total;
}

int solve2(char *input) {
    int total = 0;
    bool mul_enabled = true;

    for (int i = 0; i < strlen(input); i++) {
        if (str_lookahead(input, i, "do()")) {
            mul_enabled = true;
        }

        if (str_lookahead(input, i, "don't()")) {
            mul_enabled = false;
        }

        if (!mul_enabled) {
            continue;
        }

        if (!str_lookahead(input, i, "mul(")) {
            continue;
        }

        int n1_start = i + strlen("mul(");
        int n1_end = get_num_end(input, n1_start , ',');
        if (n1_end == -1) {
            continue;
        }

        char *str_n1 = str_slice(input, n1_start, n1_end);
        int n1 = atoi(str_n1);
        free(str_n1);

        int n2_start = n1_end + 1;
        int n2_end = get_num_end(input, n2_start , ')');
        if (n2_end == -1) {
            continue;
        }
        char *str_n2 = str_slice(input, n2_start, n2_end);
        int n2 = atoi(str_n2);
        free(str_n2);

        total = total + n1 * n2;
    }

    return total;
}

int main() {
    char *input = get_input(2024, 3, 1);
    printf("answer 1: %d\n", solve1(input));
    free(input);

    input = get_input(2024, 3, 2);
    printf("answer 2: %d\n", solve2(input));
    free(input);
}
