#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

extern const int DEFAULT_CAPACITY;

typedef struct TypeInfo TypeInfo;

typedef struct DynamicArray {
    void *data;
    int size;
    int capacity;
    int is_view;

    const TypeInfo *type;
} DynamicArray;

// Core
DynamicArray *create(const TypeInfo *type);

void destroy(DynamicArray *array);
void clear(DynamicArray *array);

void push(DynamicArray *array, const void *element);
void pop(DynamicArray *array);
void remove_at(DynamicArray *array, int index);

void *get(const DynamicArray *array, int index);
void set(DynamicArray *array, int index, const void *element);

int get_size(const DynamicArray *array);
int is_empty(const DynamicArray *array);
int get_capacity(const DynamicArray *array);

void reserve(DynamicArray *array, int new_capacity);
void shrink_to_fit(DynamicArray *array);

void print_array(const DynamicArray *array);

// Algorithms
void sort(DynamicArray* arr);

DynamicArray *copy(const DynamicArray *array);
DynamicArray *map(const DynamicArray *array, void (*func)(void*));
DynamicArray *where(const DynamicArray *array, int (*predicate)(const void*));
DynamicArray *concat(const DynamicArray *array1, const DynamicArray *array2);

#endif