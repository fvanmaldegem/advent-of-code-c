#include "str.h"

#include <stdlib.h>
#include <string.h>
#include "list.h"

char* str_slice(const char *str, int start, int end) {
    int len = end - start;
    
    char *s = malloc(len + 1);
    for (int i = start, j = 0; i < end; i++, j++) {
        if (i > strlen(str)) {
            break;
        }
        
        s[j] = str[i];
    }

    s[len] = '\0';
    return s;
}

t_list *str_split(const char *str, const char split_character) {
    t_list *splits = list_allocate(sizeof(char*));
    int start = 0;

    size_t len = strlen(str) + 1;
    for(size_t i = 0; i < len; i++) {
        const char c = str[i];

        if (c == split_character || c == '\0') {
            if (i - start == 0) {
                start = i + 1;
                continue;
            }

            char* cur_slice = str_slice(str, start, i);

            // Copy the cur_slice to a new part in memory so we can reuse cur_slice
            size_t realsize = (sizeof(char) * strlen(cur_slice)) + 1; 
            char* allocated_slice = malloc(realsize);
            strncpy(allocated_slice, cur_slice, realsize);
            
            list_append(splits, allocated_slice);
            
            // assign i + 1 to start, so it will not use the split_character
            start = i + 1;
            free(cur_slice);

        }
    }

    return splits;
}

bool str_lookahead(const char *str, int start, char *lookahead_str) {
    bool is_match = false;
    unsigned long lookahead_len = strlen(lookahead_str);
    char *compare_str = str_slice(str, start, start + lookahead_len);

    if (strcmp(compare_str, lookahead_str) == 0) {
        is_match = true;
    }

    free(compare_str);

    return is_match;
}
