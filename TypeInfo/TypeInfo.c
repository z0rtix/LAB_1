#include "TypeInfo.h"
#include "../utils/utils.h"
#include "../DynamicArray/DynamicArray.h"

#include <stdlib.h>

static TypeInfo *int_type = NULL;
static TypeInfo *double_type = NULL;
static TypeInfo *string_type = NULL;
static TypeInfo *array_type = NULL;

TypeInfo *get_int_type(void) {
    if (int_type == NULL) {
        int_type = malloc(sizeof(TypeInfo));
        *int_type = (TypeInfo){
            .element_size = sizeof(int),
            .element_free = NULL,
            .element_compare = int_compare,
            .element_print = int_print,
            .element_copy = int_copy
        };
    }
    return int_type;
}

TypeInfo *get_double_type(void) {
    if (double_type == NULL) {
        double_type = malloc(sizeof(TypeInfo));
        *double_type = (TypeInfo){
            .element_size = sizeof(double),
            .element_free = NULL,
            .element_compare = double_compare,
            .element_print = double_print,
            .element_copy = double_copy
        };
    }
    return double_type;
}

TypeInfo *get_string_type(void) {
    if (string_type == NULL) {
        string_type = malloc(sizeof(TypeInfo));
        *string_type = (TypeInfo){
            .element_size = sizeof(char*),
            .element_free = string_free,
            .element_compare = string_compare,
            .element_print = string_print,
            .element_copy = string_copy
        };
    }
    return string_type;
}

TypeInfo *get_array_type(void) {
    if (array_type == NULL) {
        array_type = malloc(sizeof(TypeInfo));
        *array_type = (TypeInfo){
            .element_size = sizeof(DynamicArray*),
            .element_free = array_free,
            .element_compare = array_compare,
            .element_print = array_print,
            .element_copy = NULL
        };
    }
    return array_type;
}

void cleanup_types(void) {
    free(int_type);
    free(double_type);
    free(string_type);
    free(array_type);
    int_type = NULL;
    double_type = NULL;
    string_type = NULL;
    array_type = NULL;
}