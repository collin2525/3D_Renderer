#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} String;

typedef struct {
    String *items;
    size_t count;
} StringArray;

/* Creation / destruction */
String string_create(const char *cstr);
StringArray string_array_create(size_t initial_capacity);
void string_free(String *s);

/* Operations */
StringArray string_split(const String *s, char delimiter);
void add_string_to_array(StringArray *arr, String s);
void remove_string_from_array(StringArray *arr, size_t index);

void string_array_free(StringArray *arr);
bool string_equals(const String *s1, const String *s2);
bool string_equals_cstr(const String *s1, const char *cstr);
bool string_contains(const String *s, const String *substr);

#endif // STRING_H