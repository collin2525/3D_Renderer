#include "string.h"
#include <stdlib.h>
#include <string.h>

String string_create(const char *cstr) {
    String s;
    s.length = strlen(cstr);
    s.capacity = s.length + 1;
    s.data = (char *)malloc(s.capacity);
    memcpy(s.data, cstr, s.capacity);
    return s;
}

void string_free(String *s) {
    free(s->data);
    s->data = NULL;
    s->length = 0;
    s->capacity = 0;
}

StringArray string_split(const String *s, char delimiter) {
    StringArray arr;
    arr.count = 0;
    arr.items = NULL;

    size_t start = 0;
    for (size_t i = 0; i <= s->length; i++) {
        if (i == s->length || s->data[i] == delimiter) {
            size_t segment_length = i - start;
            String segment;
            segment.length = segment_length;
            segment.capacity = segment_length + 1;
            segment.data = (char *)malloc(segment.capacity);
            memcpy(segment.data, s->data + start, segment_length);
            segment.data[segment_length] = '\0';

            arr.items = (String *)realloc(arr.items, sizeof(String) * (arr.count + 1));
            arr.items[arr.count] = segment;
            arr.count++;

            start = i + 1;
        }
    }

    return arr;
}

void string_array_free(StringArray *arr) {
    for (size_t i = 0; i < arr->count; i++) {
        string_free(&arr->items[i]);
    }
    free(arr->items);
    arr->items = NULL;
    arr->count = 0;
}

bool string_equals(const String *s1, const String *s2) {
    if (s1->length != s2->length) {
        return false;
    }
    return (memcmp(s1->data, s2->data, s1->length) == 0);
}

bool string_equals_cstr(const String *s1, const char *cstr) {
    size_t cstr_length = strlen(cstr);
    if (s1->length != cstr_length) {
        return false;
    }
    return (memcmp(s1->data, cstr, s1->length) == 0);
}

bool string_contains(const String *s, const String *substr) {
    if (substr->length == 0 || substr->length > s->length) {
        return false;
    }
    for (size_t i = 0; i <= s->length - substr->length; i++) {
        if (memcmp(s->data + i, substr->data, substr->length) == 0) {
            return true;
        }
    }
    return false;
}

void add_string_to_array(StringArray *arr, String s) {
    arr->items = (String *)realloc(arr->items, sizeof(String) * (arr->count + 1));
    arr->items[arr->count] = s;
    arr->count++;
}

void remove_string_from_array(StringArray *arr, size_t index) {
    if (index >= arr->count) {
        return;
    }
    string_free(&arr->items[index]);
    for (size_t i = index; i < arr->count - 1; i++) {
        arr->items[i] = arr->items[i + 1];
    }
    arr->count--;
    arr->items = (String *)realloc(arr->items, sizeof(String) * arr->count);
}