#ifndef UTILS_H
#define UTILS_H

int int_compare(const void *a, const void *b);
void int_print(const void *x);
void *int_copy(const void *x);
void int_map_square(void *x);
void int_map_cube(void *x);
void int_map_add10(void *x);
int int_is_even(const void *x);
int int_is_odd(const void *x);
int int_gt_5(const void *x);

int double_compare(const void *a, const void *b);
void double_print(const void *x);
void *double_copy(const void *x);
void double_map_square(void *x);
void double_map_sqrt(void *x);
void double_map_mul2(void *x);
int double_is_positive(const void *x);
int double_gt_1(const void *x);

int string_compare(const void *a, const void *b);
void string_print(const void *x);
void string_free(void *x);
void *string_copy(const void *x);
void string_map_upper(void *x);
void string_map_exclaim(void *x);
void string_map_reverse(void *x);
int string_length_gt_3(const void *x);
int string_starts_with_a(const void *x);
int string_contains_e(const void *x);

void array_free(void *x);
void array_print(const void *x);
int array_compare(const void *a, const void *b);

// extern const TypeInfo INT_TYPE;
// extern const TypeInfo DOUBLE_TYPE;
// extern const TypeInfo STRING_TYPE;
// extern const TypeInfo ARRAY_TYPE;

#endif
