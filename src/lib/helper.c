#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <curl/header.h>
#include <time.h>
#include <stdarg.h>

#include "list.h"
#include "str.h"

const char* AOC_URL          = "https://adventofcode.com";
const char* INPUT_URL_FORMAT = "%s/%d/day/%d/input";

struct MemoryStruct {
    char *memory;
    size_t size;
};

/*
Function for Curl to write a response into memory
*/
size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        printf("error: not enough memory\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

void panic(int exit_code, char* message) {
    void *array[10];
    int size;

    if (message != NULL) {
        fprintf(stdout, "panic: %s", message);
    }

    size = backtrace (array, 10);
    backtrace_symbols_fd(array, size, fileno(stdout));

    exit(exit_code);
}

char* get_token() {
    char* token = getenv("SESSION_COOKIE");
    if (token == NULL) {
        return "";
    }

    return token;
}

char* get_input_url(int year, int day) {
    char *url = malloc(sizeof(char) * 300);
    int result = sprintf(url, INPUT_URL_FORMAT, AOC_URL, year, day);
    if (result == 0) {
        return "";
    }

    return url;
}

char* get_input(int year, int day) {
    char* url = get_input_url(year, day);
    char* token = get_token();
    char* token_cookie = malloc(sizeof(char) * 300);
    
    CURL *curl_handle;
    CURLcode curl_code;
    
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    sprintf(token_cookie, "session=%s", token);
    curl_handle = curl_easy_init();
    if (curl_handle != NULL) {
        curl_easy_setopt(curl_handle, CURLOPT_COOKIE, token_cookie);
        curl_easy_setopt(curl_handle, CURLOPT_URL, url);
        curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "advent-of-code-c-agent/1.0");

        curl_code = curl_easy_perform(curl_handle);
        if (curl_code != CURLE_OK) {
            char* message = malloc(sizeof(char) * 3000);
            sprintf(message, "could not execute curl request, got curl code %d", curl_code);
            panic(1, message);
        }

        curl_easy_cleanup(curl_handle);
    } else {
        panic(1, "curl was not initialized");
    }

    free(token_cookie);
    free(url);
    
    return chunk.memory;
}

void parse_int_columns(char *text, int columns, ...) {
    va_list args;
    va_start(args, columns);

    t_list *lists = list_allocate(sizeof(t_list*));
    for (int i = 0; i < columns; i++) {
        t_list *l = va_arg(args, t_list*);
        list_append(lists, l);
    }

    va_end(args);

    t_list *lines = str_split(text, '\n');
    for(int i = 0; i < lines->len; i++) {
        const char* cur_line = (char*) list_get_by_index(lines, i)->data;
        t_list *line_columns = str_split(cur_line, ' ');

        for (int j = 0; j < columns; j++) {
            int intv  = atoi(list_get_by_index(line_columns, j)->data);
            int *intp = malloc(sizeof(int));
            memcpy(intp, &intv, sizeof(int));
            list_append(list_get_by_index(lists, j)->data, intp);
        }

        list_free(&line_columns, free);
    }

    list_free(&lines, free);
    list_free(&lists, 0);
}