#include <stdio.h>
#include <stdlib.h>

#include "str.h"

#define BUFFER_SIZE 50

size_t string_length(string_t str) {
    size_t len = 0;

    while (str[len] != '\0') len++;

    return len;
}

void new_line() {
    printf("\n");
}

void new_lines(unsigned n) {
    for (unsigned i = 0; i < n; i++) {
        printf("\n");
    }
}

void print_string(string_t str) {
    printf("%s", str);
}

void println_string(string_t str) {
    printf("%s\n", str);
}

symbol_t lower_case_char(symbol_t c) {
    int diff = 'a' - 'A';

    if (c >= 'A' && c <= 'Z') {
        return c + diff;
    }

    return c;
}

string_t new_string_to_lower(string_t str) {
    int n = string_length(str);

    string_t res_str = (string_t)malloc((n + 1) * sizeof(symbol_t));

    res_str[n] = '\0';

    for (int i = 0; i < n; i++) {
        res_str[i] =  lower_case_char(str[i]);
    }

    return res_str;
}

int compare_strings_non_case_sensitive(string_t str1, string_t str2) {
    int diff = 0;

    int i = 0;

    string_t str1_comp = new_string_to_lower(str1);
    string_t str2_comp = new_string_to_lower(str2);

    while (str1_comp[i] != '\0' && str2_comp[i] != '\0' && diff == 0) {
        diff += str1_comp[i] - str2_comp[i];
        i++;
    }

    if (diff == 0) {
        diff = string_length(str1_comp) - string_length(str2_comp);
    }

    destroy_string(str1_comp);
    destroy_string(str2_comp);

    return diff;
}

int compare_strings_case_sensitive(string_t str1, string_t str2) {
    int diff = 0;

    int i = 0;

    while (str1[i] != '\0' && str2[i] != '\0' && diff == 0) {
        diff += str1[i] - str2[i];
        i++;
    }

    if (diff == 0) {
        diff = string_length(str1) - string_length(str2);
    }

    return diff;
}

void *copy_memory(void *dest, void *src, size_t n_bytes) {
	char *d = (char *)dest;
	char *s = (char *)src;

	for (int i = 0; i < (int)n_bytes; i++) {
		d[i] = s[i];
	}

	return dest;
}

string_t concatenate_strings(string_t dest, string_t src) {
    copy_memory(&dest[string_length(dest)], src, string_length(src) + 1);

    return dest;
}

void static_read_until(string_t str, symbol_t separator) {
    char c;

    int i = 0;
    while ((c = getchar()) != ' ' && c != '\n' && c != '\r' && c != EOF) {
        str[i] = c;

        i++;
    }

    str[i] = '\0';
}

string_t read_until(FILE *stream, symbol_t separator, int *has_EOF) {
    char c;
    unsigned long int n_chars = 0;

    string_t line = NULL;

    while ((c = fgetc(stream)) == '\n' || (c == '\r') || (c == separator));

    if (c != EOF) ungetc(c, stream);

    do {
        c = fgetc(stream);

        if (n_chars % BUFFER_SIZE == 0) {
            unsigned long int new_size = (n_chars / BUFFER_SIZE + 1) * BUFFER_SIZE + 1;

            line = (string_t)realloc(line, (size_t)new_size * sizeof(symbol_t));
        }

        n_chars++;

        if (c != '\n' && c != '\r' && c != separator && c != EOF) {
            *(line + n_chars - 1) = c;

            *has_EOF = 0;
        } else {
            *(line + n_chars - 1) = '\0';

            if (c == EOF) *has_EOF = 1;
        }
    } while (c != '\n' && c != '\r' && c != separator && c != EOF);

    line = (string_t)realloc(line, (size_t)(n_chars + 1));

    return line;
}

void static_read_line(string_t str) {
    char c;

    int i = 0;
    while ((c = getchar()) != '\r' && c != '\n' && c != EOF) {
        str[i] = c;

        i++;
    }

    str[i] = '\0';
}

string_t read_line(FILE *stream, int *has_EOF) {
    char c;
    unsigned long int n_chars = 0;

    string_t line = NULL;

    while ((c = fgetc(stream)) == '\n' || (c == '\r'));

    if (c != EOF) ungetc(c, stream);

    do {
        c = fgetc(stream);

        if (n_chars % BUFFER_SIZE == 0) {
            int new_size = (n_chars / BUFFER_SIZE + 1) * BUFFER_SIZE + 1;

            line = (string_t)realloc(line, (size_t)new_size * sizeof(symbol_t));
        }

        n_chars++;

        if (c != '\n' && c != '\r' && c != EOF) {
            *(line + n_chars - 1) = c;

            *has_EOF = 0;
        } else {
            *(line + n_chars - 1) = '\0';

            if (c == EOF) *has_EOF = 1;
        }
    } while (c != '\n' && c != '\r' && c != EOF);

    line = (string_t)realloc(line, (size_t)(n_chars + 1));

    return line;
}

void destroy_string(string_t str) {
    if (str) free(str);
}

string_t reverse_string(string_t str) {
    int n = string_length(str);

    string_t res_str = (string_t)malloc((n + 1) * sizeof(symbol_t));

    res_str[n] = '\0';

    for (int i = n - 1; i >= 0; i--) {
        res_str[n - 1 - i] =  str[i];
    }

    destroy_string(str);

    return res_str;
}

string_t create_empty_string() {
    string_t str = (string_t)malloc(1 * sizeof(symbol_t));

    str[0] = '\0';

    return str;
}

void add_symbol_to_string(string_t *str, symbol_t new_symbol) {
    int len = string_length(*str);

    *str = (string_t)realloc(*str, (len + 2) * sizeof(symbol_t));

    (*str)[len] = new_symbol;
    (*str)[len + 1] = '\0';
}

string_t *split_string(string_t str, symbol_t separator, int *n_elems) {
    *n_elems = 1;

    string_t *elements = (string_t *)malloc((*n_elems) * sizeof(string_t));
    
    int i = 0;
    int coming_from_separator = 0;
    do {
        coming_from_separator = 0;
        string_t cur_elem = create_empty_string();
        while (str[i] != '\0' && str[i] != separator) {
            add_symbol_to_string(&cur_elem, str[i]);
            i++;
        }

        elements[*n_elems - 1] = cur_elem;

        if (str[i] == separator) {
            i++;
            (*n_elems)++;

            elements = (string_t *)realloc(elements, (*n_elems) * sizeof(string_t));
            coming_from_separator = 1;
        }
    } while (str[i] != '\0' || coming_from_separator);
    
    return elements;
}

void destroy_string_array(string_t *string_array, int n_elems) {
    for (int i = 0; i < n_elems; i++) {
        destroy_string(string_array[i]);
    }

    free(string_array);
}

void remove_quotes(string_t string) {
    if(string[0] != "\"")
        return;
    
    int size = string_length(string);
    for(int i = 0; i < size - 2; i++) {
        string[i] = string[i+1];
    }
    string[size - 2] = '\0';
}