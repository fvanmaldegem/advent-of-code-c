#include "stdlib.h"
#include "../lib/helper.h"
#include "../lib/list.h"
#include <curl/curl.h>
#include <stdio.h>

bool is_valid_report(t_list *report) {

    int *previous_level = NULL;
    bool direction_set = false;
    bool is_increasing = false;
    
    for (int j = 0; j < report->len; j++) {
        int *current_level  = (int *) list_get_by_index(report, j)->data;

        if (previous_level == NULL) {
            previous_level = current_level;
            continue;
        }

        if (!direction_set) {
            is_increasing = *previous_level < *current_level;
            direction_set = true;
        } else if (is_increasing != (*previous_level < *current_level)) {
            return false;
        }

        int diff = abs(*previous_level - *current_level);
        if (diff < 1 || diff > 3) {
            return false;
        }

        previous_level = current_level;
    }

    return true;
}

int solve1(char *input) {
    t_list *reports = parse_int_matrix(input);
    int valid_reports = 0;

    for (int i = 0; i < reports->len; i++) {
        t_list *report = (t_list *) list_get_by_index(reports, i)->data;

        if (is_valid_report(report)) {
            valid_reports++;
        }

        // Free the data in the row, since we won't need it anymore
        list_free(&report, free);
    }

    list_free(&reports, 0);

    return valid_reports;
}

int solve2(char* input) {
    t_list *reports = parse_int_matrix(input);
    int valid_reports = 0;

    for (int i = 0; i < reports->len; i++) {
        t_list *report = (t_list *) list_get_by_index(reports, i)->data;

        if (is_valid_report(report)) {
            valid_reports++;
        } else {
            for (int j = 0; j < report->len; j++) {
                t_list *dampened_report = list_clone(report,true);

                // We still need to clean up the data after removing it from the list.
                void *dangling_value = list_remove_by_index(dampened_report, j);
                free(dangling_value);

                if (is_valid_report(dampened_report)) {
                    valid_reports++;
                    list_free(&dampened_report, free);
                    break;
                }

                list_free(&dampened_report, free);
            }
        }

        // Free the data in the row, since we won't need it anymore
        list_free(&report, free);
    }

    list_free(&reports, 0);

    return valid_reports;
}

int main() {
    char *input = get_input(2024, 2, 0);
    printf("answer 1: %d\n", solve1(input));
    printf("answer 2: %d\n", solve2(input));
    free(input);
}