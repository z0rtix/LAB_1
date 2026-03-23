#ifndef TYPEINFO_H
#define TYPEINFO_H

typedef struct TypeInfo {
    int element_size;
    
    void (*element_free)(void*);
    int (*element_compare)(const void*, const void*);
    void (*element_print)(const void*);
    void *(*element_copy)(const void*);
} TypeInfo;

TypeInfo* get_int_type(void);
TypeInfo* get_double_type(void);
TypeInfo* get_string_type(void);
TypeInfo* get_array_type(void);

void cleanup_types(void);

#endif