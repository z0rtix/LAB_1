#include "../DynamicArray/DynamicArray.h"

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// for int
int int_compare(const void *a, const void *b) {
    int va = *(const int*)a;
    int vb = *(const int*)b;
    return (va > vb) - (va < vb);
}

void int_print(const void *x) {
    printf("%d", *(const int*)x);
}

void *int_copy(const void *x) {
    int *p = malloc(sizeof(int));
    *p = *(const int*)x;
    return p;
}

void int_map_square(void *x) {
    *(int*)x = *(int*)x * *(int*)x;
}

void int_map_cube(void *x) {
    int val = *(int*)x;
    *(int*)x = val * val * val;
}

void int_map_add10(void *x) {
    *(int*)x += 10;
}

int int_is_even(const void *x) {
    return (*(const int*)x) % 2 == 0;
}

int int_is_odd(const void *x) {
    return (*(const int*)x) % 2 != 0;
}

int int_gt_5(const void *x) {
    return *(const int*)x > 5;
}

// const TypeInfo INT_TYPE = {
//     .element_size = sizeof(int),
//     .element_free = NULL,
//     .element_compare = int_compare,
//     .element_print = int_print,
//     .element_copy = int_copy
// };


// for double
int double_compare(const void *a, const void *b) {
    double diff = *(const double*)a - *(const double*)b;
    return (diff > 0) - (diff < 0);
}

void double_print(const void *x) {
    printf("%.2f", *(const double*)x);
}

void *double_copy(const void *x) {
    double *p = malloc(sizeof(double));
    *p = *(const double*)x;
    return p;
}

void double_map_square(void *x) {
    double val = *(double*)x;
    *(double*)x = val * val;
}

void double_map_sqrt(void *x) {
    double val = *(double*)x;
    *(double*)x = sqrt(val);
}

void double_map_mul2(void *x) {
    *(double*)x *= 2;
}

int double_is_positive(const void *x) {
    return *(const double*)x > 0;
}

int double_gt_1(const void *x) {
    return *(const double*)x > 1.0;
}

// const TypeInfo DOUBLE_TYPE = {
//     .element_size = sizeof(double),
//     .element_free = NULL,
//     .element_compare = double_compare,
//     .element_print = double_print,
//     .element_copy = double_copy
// };


// for str
int string_compare(const void *a, const void *b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

void string_print(const void *x) {
    printf("%s", *(const char**)x);
}

void string_free(void *x) {
    // printf("FREE: %s\n", *(char**)x);
    free(*(char**)x);
}

void *string_copy(const void *x) {
    char **dst_ptr = malloc(sizeof(char*));
    *dst_ptr = strdup(*(const char**)x);
    return dst_ptr;
}

void string_map_upper(void *x) {
    char **s = (char**)x;
    char *old = *s;
    char *new_str = malloc(strlen(old) + 1);

    if (new_str) {
        strcpy(new_str, old);
        for (int i = 0; new_str[i]; i++) {
            new_str[i] = toupper(new_str[i]);
        }
        *s = new_str;
        free(old);
    }
}

void string_map_exclaim(void *x) {
    char **s = (char**)x;
    char *old = *s;
    char *new_str = malloc(strlen(old) + 4);

    if (new_str) {
        strcpy(new_str, old);
        strcat(new_str, "!!!");
        *s = new_str;
        free(old);
    }
}

void string_map_reverse(void *x) {
    char **s = (char**)x;
    char *old = *s;
    int len = strlen(old);
    char *new_str = malloc(len + 1);

    if (new_str) {
        for (int i = 0; i < len; i++) {
            new_str[i] = old[len - 1 - i];
        }
        new_str[len] = '\0';
        *s = new_str;
        free(old);
    }
}

int string_length_gt_3(const void *x) {
    return strlen(*(char**)x) > 3;
}

int string_starts_with_a(const void *x) {
    char *s = *(char**)x;
    return s[0] == 'A' || s[0] == 'a';
}

int string_contains_e(const void *x) {
    char *s = *(char**)x;
    return strchr(s, 'e') != NULL || strchr(s, 'E') != NULL;
}

// const TypeInfo STRING_TYPE = {
//     .element_size = sizeof(char*),
//     .element_free = string_free,
//     .element_compare = string_compare,
//     .element_print = string_print,
//     .element_copy = string_copy
// };


// for array of arrays
void array_free(void *x) {
    if (x == NULL || *(DynamicArray**)x == NULL) return;
    destroy(*(DynamicArray**)x);
}

void array_print(const void *x) {
    const DynamicArray *inner = *(const DynamicArray**)x;
    print_array(inner);
}

int array_compare(const void *a, const void *b) {
    const DynamicArray *arr1 = *(const DynamicArray**)a;
    const DynamicArray *arr2 = *(const DynamicArray**)b;

    if (get_size(arr1) != get_size(arr2)) {
        return get_size(arr1) - get_size(arr2);
    }
    return 0;
}

// const TypeInfo ARRAY_TYPE = {
//     .element_size = sizeof(DynamicArray*),
//     .element_free = array_free,
//     .element_compare = array_compare,
//     .element_print = array_print
// };