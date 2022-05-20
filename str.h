// Personal Library to Manipulate Strings

typedef unsigned long size_t;
typedef char symbol_t;
typedef symbol_t * string_t;

size_t string_length(string_t str);
void new_line();
void new_lines(unsigned n);
void print_string(string_t str);
void println_string(string_t str);
symbol_t lower_case_char(symbol_t c);
string_t new_string_to_lower(string_t str);
int compare_strings_non_case_sensitive(string_t str1, string_t str2);
int compare_strings_case_sensitive(string_t str1, string_t str2);
void *copy_memory(void *dest, void *src, size_t n_bytes);
string_t concatenate_strings(string_t dest, string_t src);
void static_read_until(string_t str, symbol_t separator);
string_t read_until(FILE *stream, symbol_t separator, int *has_EOF);
void static_read_line(string_t str);
string_t read_line(FILE *stream, int *has_EOF);
void destroy_string(string_t str);
string_t reverse_string(string_t str);
string_t create_empty_string();
void add_symbol_to_string(string_t *str, symbol_t new_symbol);
string_t *split_string(string_t str, symbol_t separator, int *n_elems);
void destroy_string_array(string_t *string_array, int n_elems);
void scan_quote_string(string_t str);
void remove_quotes(string_t string);