#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "../lib/helper.h"
#include "../lib/list.h"

int smallest_int_index(t_list *list) {
    if (list == NULL) {
        return -1;
    }

    if (list->len == 0) {
        return -1;
    }

    int *pv = (int*) list->head->data;

    for (int i = 0; i < list->len; i++) {
        int *pcv = (int*) list_get_by_index(list, i)->data;
        if (*(int*) list_get_by_index(list, i)->data < *pv) {
            pv = pcv;
        }
    }

    return list_get_index(list, pv);
}

int solve1(char* input) {
    t_list *left = list_allocate(sizeof(int));
    t_list *right = list_allocate(sizeof(int));

    parse_int_columns(input, 2, left, right);

    int total_distance = 0;
    while (left->len > 0) {
        int i_left = smallest_int_index(left);
        int i_right = smallest_int_index(right);

        if (i_left == -1 || i_right == -1) {
            break;
        }

        int *p_left = list_remove_by_index(left, i_left);
        int *p_right = list_remove_by_index(right, i_right);

        total_distance += abs(*p_left - *p_right);
        free(p_left); free(p_right);
    }

    list_free(&left, free);
    list_free(&right, free);

    return total_distance;
}

int solve2(char *input) {
    t_list *left = list_allocate(sizeof(int));
    t_list *right = list_allocate(sizeof(int));

    parse_int_columns(input, 2, left, right);

    int similarity_score = 0;
    for (int i = 0; i < left->len; i++) {
        int v_left = *(int *) list_get_by_index(left, i)->data;

        int occurences = 0;
        for (int j = 0; j < right->len; j++) {
            int v_right = *(int *) list_get_by_index(right, j)->data;

            if (v_right == v_left) {
                occurences++;
            }
        }

        similarity_score += v_left * occurences;
    }

    list_free(&left, free);
    list_free(&right, free);
    return similarity_score;
}

int main() {
    char *input = get_input(2024, 1, 0);
    printf("answer 1: %d\n", solve1(input));
    printf("answer 2: %d\n", solve2(input));
    free(input);
}
