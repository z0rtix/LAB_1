#include "DynamicArray.h"
#include "../TypeInfo/TypeInfo.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void sort(DynamicArray* array) {
    if (array == NULL || array->size < 2 || array->type->element_compare == NULL || array->is_view) return;
    char *temp = malloc(array->type->element_size);
    if (temp == NULL) return;

    for (int i = 0; i < array->size - 1; i++) {
        for (int j = 0; j < array->size - 1 - i; j++) {
            void *current_element = (char*)array->data + j * array->type->element_size;
            void *next_element = (char*)array->data + (j + 1) * array->type->element_size;
            if (array->type->element_compare(current_element, next_element) > 0) {                
                memcpy(temp, current_element, array->type->element_size);
                memcpy(current_element, next_element, array->type->element_size);
                memcpy(next_element, temp, array->type->element_size);
            }
        }
    }
    free(temp);
}

DynamicArray *copy(const DynamicArray *array) {
    if (array == NULL) return NULL;
    DynamicArray *copy_of_array = create(array->type);
    copy_of_array->is_view = 0;
    if (copy_of_array == NULL) return NULL;
    reserve(copy_of_array, array->size);
    for (int i = 0; i < array->size; i++) {
        void *current_of_array = (char*)array->data + i * array->type->element_size;
        void *copy_of_current = array->type->element_copy(current_of_array);
        if (copy_of_current == NULL) {
            destroy(copy_of_array);
            return NULL;
        }
        push(copy_of_array, copy_of_current);
        free(copy_of_current);
        // array->type->element_free(copy_of_current);
    }

    return copy_of_array;
}

DynamicArray* map(const DynamicArray *array, void (*func)(void*)) {
    if (array == NULL || func == NULL) return NULL;
    DynamicArray *array_map = copy(array);
    if (array_map == NULL) return NULL;
    for (int i = 0; i < array_map->size; i++) {
        void *current = (char*)array_map->data + i * array_map->type->element_size;
        func(current);
    }

    return array_map;
}

DynamicArray* where(const DynamicArray* array, int (*predicate)(const void*)) {
    if (array == NULL || predicate == NULL) return NULL;

    DynamicArray *result = create(array->type);

    for (int i = 0; i < array->size; i++) {
        void *element = (char*)array->data + i * array->type->element_size;
        if (predicate(element)) {
            push(result, element);
        }
    }

    result->is_view = 1;

    return result;
}

DynamicArray* concat(const DynamicArray* array1, const DynamicArray* array2) {
    if (array1 == NULL || array2 == NULL || array1->type != array2->type) return NULL;

    DynamicArray* concat_array = create(array1->type);

    for (int i = 0; i < array1->size; i++) {
        void *element = (char*)array1->data + i * array1->type->element_size;
        push(concat_array, element);
    }

    for (int i = 0; i < array2->size; i++) {
        void *element = (char*)array2->data + i * array2->type->element_size;
        push(concat_array, element);
    }

    concat_array->is_view = 1;

    return concat_array;
}
