#include "DynamicArray/DynamicArray.h"
#include "TypeInfo/TypeInfo.h"
#include "utils/utils.h"
#include "tests.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>


static void test_core(const TypeInfo *type, void **samples, int n, const char *name) {
    DynamicArray *arr = create(type);
    
    assert(arr);
    assert(get_size(arr) == 0);
    assert(is_empty(arr));

    for (int i = 0; i < n; i++) {
        void *cpy = type->element_copy ? type->element_copy(samples[i]) : samples[i];
        push(arr, cpy);
        if (type->element_copy) free(cpy);
    }

    assert(get_size(arr) == n);
    for (int i = 0; i < n; i++)
        assert(type->element_compare(get(arr, i), samples[i]) == 0);

    pop(arr);
    assert(get_size(arr) == n - 1);

    if (n >= 2)
        assert(type->element_compare(get(arr, n - 2), samples[n - 2]) == 0);

    if (n >= 2) {
        remove_at(arr, 0);
        assert(get_size(arr) == n - 2);
        if (n >= 3)
            assert(type->element_compare(get(arr, 0), samples[1]) == 0);
    }

    if (get_size(arr) > 0 && n >= 2) {
        void *last_cpy = type->element_copy ? type->element_copy(samples[n - 1]) : samples[n - 1];
        set(arr, 0, last_cpy);
        if (type->element_copy) free(last_cpy);
        assert(type->element_compare(get(arr, 0), samples[n - 1]) == 0);
    }

    clear(arr);
    assert(get_size(arr) == 0);
    assert(get_capacity(arr) == DEFAULT_CAPACITY);
    assert(arr->data != NULL);

    int cap = get_capacity(arr);
    reserve(arr, cap + 5);
    assert(get_capacity(arr) >= cap + 5);

    void *first_cpy = type->element_copy ? type->element_copy(samples[0]) : samples[0];
    push(arr, first_cpy);
    if (type->element_copy) free(first_cpy);
    assert(get_size(arr) == 1);

    reserve(arr, 1);
    assert(get_capacity(arr) >= cap + 5);

    while (get_size(arr) < get_capacity(arr)) {
        void *cpy = type->element_copy ? type->element_copy(samples[0]) : samples[0];
        push(arr, cpy);
        if (type->element_copy) free(cpy);
    }

    int before = get_size(arr);
    void *expand_cpy = type->element_copy ? type->element_copy(samples[0]) : samples[0];
    push(arr, expand_cpy);

    if (type->element_copy) free(expand_cpy);
    assert(get_size(arr) == before + 1);
    assert(get_capacity(arr) > before);

    while (get_size(arr) > 5) pop(arr);
    int sz = get_size(arr);
    shrink_to_fit(arr);
    assert(get_capacity(arr) == sz);
    assert(get(arr, -1) == NULL);
    assert(get(arr, get_size(arr)) == NULL);

    assert(get_size(arr) > 0);
    void *ignored = samples[0];
    set(arr, -1, ignored);
    set(arr, 999, ignored);
    assert(type->element_compare(get(arr, 0), samples[0]) == 0);

    destroy(arr);
    printf("✅ Тесты ядра (%s) пройдены\n", name);
}


static void test_algorithms(const TypeInfo *type, void **unsorted, void **sorted, int n, void (*map_func)(void*), int (*predicate)(const void*), const char *name) {
    DynamicArray *arr = create(type);

    for (int i = 0; i < n; i++) {
        void *cpy = type->element_copy ? type->element_copy(unsorted[i]) : unsorted[i];
        push(arr, cpy);
        if (type->element_copy) free(cpy);
    }

    sort(arr);
    for (int i = 0; i < n; i++)
        assert(type->element_compare(get(arr, i), sorted[i]) == 0);
    destroy(arr);

    arr = create(type);
    for (int i = 0; i < n; i++) {
        void *cpy = type->element_copy ? type->element_copy(unsorted[i]) : unsorted[i];
        push(arr, cpy);
        if (type->element_copy) free(cpy);
    }

    DynamicArray *cp = copy(arr);
    assert(get_size(cp) == n);

    for (int i = 0; i < n; i++)
        assert(type->element_compare(get(arr, i), get(cp, i)) == 0);

    void *last_cpy = type->element_copy ? type->element_copy(sorted[0]) : sorted[0];
    set(arr, 0, last_cpy);

    if (type->element_copy) free(last_cpy);
    assert(type->element_compare(get(arr, 0), sorted[0]) == 0);
    assert(type->element_compare(get(cp, 0), unsorted[0]) == 0);
    destroy(cp);
    destroy(arr);

    arr = create(type);
    for (int i = 0; i < n; i++) {
        void *cpy = type->element_copy ? type->element_copy(unsorted[i]) : unsorted[i];
        push(arr, cpy);
        if (type->element_copy) free(cpy);
    }

    DynamicArray *mapped = map(arr, map_func);
    assert(get_size(mapped) == n);
    assert(type->element_compare(get(mapped, 0), unsorted[0]) != 0);
    destroy(mapped);
    destroy(arr);

    arr = create(type);
    for (int i = 0; i < n; i++) {
        void *cpy = type->element_copy ? type->element_copy(unsorted[i]) : unsorted[i];
        push(arr, cpy);
        if (type->element_copy) free(cpy);
    }
    DynamicArray *filtered = where(arr, predicate);
    assert(get_size(filtered) >= 1);
    for (int i = 0; i < get_size(filtered); i++)
        assert(predicate(get(filtered, i)) == 1);
    destroy(filtered);
    destroy(arr);

    DynamicArray *a1 = create(type);
    DynamicArray *a2 = create(type);

    for (int i = 0; i < n/2; i++) {
        void *cpy = type->element_copy ? type->element_copy(unsorted[i]) : unsorted[i];
        push(a1, cpy);
        if (type->element_copy) free(cpy);
    }

    for (int i = n/2; i < n; i++) {
        void *cpy = type->element_copy ? type->element_copy(unsorted[i]) : unsorted[i];
        push(a2, cpy);
        if (type->element_copy) free(cpy);
    }

    DynamicArray *cc = concat(a1, a2);
    assert(get_size(cc) == n);

    for (int i = 0; i < n; i++)
        assert(type->element_compare(get(cc, i), unsorted[i]) == 0);
    
    destroy(a1); destroy(a2); destroy(cc);


    if (type == get_int_type()) {
        DynamicArray *int_arr = create(get_int_type());

        int dummy = 0;
        push(int_arr, &dummy);
        DynamicArray *str_arr = create(get_string_type());

        char *fake = strdup("fake");
        push(str_arr, &fake);
        assert(concat(int_arr, str_arr) == NULL);

        destroy(int_arr);
        destroy(str_arr);
    }

    printf("✅ Тесты алгоритмов (%s) пройдены\n", name);
}


static void test_stress(const TypeInfo *type, void **prototypes, int proto_cnt, const char *name) {
    clock_t start = clock();

    DynamicArray *arr = create(type);
    reserve(arr, 10000000);

    for (int i = 0; i < 10000000; i++) {
        void *cpy = type->element_copy(prototypes[i % proto_cnt]);
        push(arr, cpy);
        free(cpy);
    }

    destroy(arr);

    double t = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("✅ Стресс‑тест 10M (%s) пройден за %.3f с\n", name, t);
}


void test_int_arrays() {
    printf("Тест массива массивов int -- ");
    clock_t start = clock();
    DynamicArray *arr_of_arrs = create(get_array_type());
    assert(arr_of_arrs != NULL);
    assert(get_size(arr_of_arrs) == 0);

    DynamicArray *row1 = create(get_int_type());
    push(row1, &(int){1});
    push(row1, &(int){2});
    push(row1, &(int){3});
    assert(get_size(row1) == 3);
    assert(*(int*)get(row1, 0) == 1);
    assert(*(int*)get(row1, 2) == 3);

    DynamicArray *row2 = create(get_int_type());
    push(row2, &(int){4});
    push(row2, &(int){5});
    push(row2, &(int){6});
    assert(get_size(row2) == 3);

    DynamicArray *row3 = create(get_int_type());
    push(row3, &(int){4});
    push(row3, &(int){5});
    push(row3, &(int){6});
    assert(get_size(row3) == 3);

    DynamicArray *row4 = create(get_int_type());
    push(row4, &(int){7});
    push(row4, &(int){8});
    push(row4, &(int){9});
    assert(get_size(row4) == 3);

    DynamicArray *row5 = create(get_int_type());
    push(row5, &(int){11});
    push(row5, &(int){12});
    push(row5, &(int){13});
    push(row5, &(int){133});
    assert(get_size(row5) == 4);
    assert(*(int*)get(row5, 3) == 133);

    push(arr_of_arrs, &row1);
    push(arr_of_arrs, &row2);
    push(arr_of_arrs, &row3);
    push(arr_of_arrs, &row4);
    push(arr_of_arrs, &row5);
    
    assert(get_size(arr_of_arrs) == 5);
    DynamicArray *first_row = *(DynamicArray**)get(arr_of_arrs, 0);
    assert(*(int*)get(first_row, 0) == 1);
    DynamicArray *last_row = *(DynamicArray**)get(arr_of_arrs, 4);
    assert(*(int*)get(last_row, 3) == 133);
    assert(*(int*)get(row5, 0) == 11);
    assert(*(int*)get(row5, 1) == 12);
    assert(*(int*)get(row5, 2) == 13);
    assert(*(int*)get(row5, 3) == 133);

    destroy(arr_of_arrs);
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("✅ (%.4f сек)\n", time_spent);
}


void test_string_arrays() {
    printf("Тест массива массивов string -- ");
    clock_t start = clock();
    
    DynamicArray *arr_of_str_arrays = create(get_array_type());
    assert(arr_of_str_arrays != NULL);
    assert(get_size(arr_of_str_arrays) == 0);
    
    DynamicArray *words1 = create(get_string_type());
    char *s1_1 = strdup("apple");
    char *s1_2 = strdup("banana");
    char *s1_3 = strdup("cherry");
    push(words1, &s1_1);
    push(words1, &s1_2);
    push(words1, &s1_3);
    assert(get_size(words1) == 3);
    assert(strcmp(*(char**)get(words1, 0), "apple") == 0);
    assert(strcmp(*(char**)get(words1, 2), "cherry") == 0);

    DynamicArray *words2 = create(get_string_type());
    char *s2_1 = strdup("dog");
    char *s2_2 = strdup("cat");
    char *s2_3 = strdup("hamster");
    push(words2, &s2_1);
    push(words2, &s2_2);
    push(words2, &s2_3);
    assert(get_size(words2) == 3);

    DynamicArray *words3 = create(get_string_type());
    char *s3_1 = strdup("red");
    char *s3_2 = strdup("green");
    char *s3_3 = strdup("blue");
    push(words3, &s3_1);
    push(words3, &s3_2);
    push(words3, &s3_3);
    assert(get_size(words3) == 3);

    DynamicArray *words4 = create(get_string_type());
    char *s4_1 = strdup("one");
    char *s4_2 = strdup("two");
    char *s4_3 = strdup("three");
    char *s4_4 = strdup("four");
    push(words4, &s4_1);
    push(words4, &s4_2);
    push(words4, &s4_3);
    push(words4, &s4_4);
    assert(get_size(words4) == 4);

    DynamicArray *words5 = create(get_string_type());
    char *s5_1 = strdup("alpha");
    char *s5_2 = strdup("beta");
    char *s5_3 = strdup("gamma");
    char *s5_4 = strdup("delta");
    char *s5_5 = strdup("epsilon");
    push(words5, &s5_1);
    push(words5, &s5_2);
    push(words5, &s5_3);
    push(words5, &s5_4);
    push(words5, &s5_5);
    assert(get_size(words5) == 5);
    assert(strcmp(*(char**)get(words5, 4), "epsilon") == 0);
    
    push(arr_of_str_arrays, &words1);
    push(arr_of_str_arrays, &words2);
    push(arr_of_str_arrays, &words3);
    push(arr_of_str_arrays, &words4);
    push(arr_of_str_arrays, &words5);
    assert(get_size(arr_of_str_arrays) == 5);
    
    DynamicArray *first_array = *(DynamicArray**)get(arr_of_str_arrays, 0);
    assert(first_array != NULL);
    assert(get_size(first_array) == 3);
    assert(strcmp(*(char**)get(first_array, 0), "apple") == 0);
    assert(strcmp(*(char**)get(first_array, 1), "banana") == 0);
    assert(strcmp(*(char**)get(first_array, 2), "cherry") == 0);

    DynamicArray *last_array = *(DynamicArray**)get(arr_of_str_arrays, 4);
    assert(last_array != NULL);
    assert(get_size(last_array) == 5);
    assert(strcmp(*(char**)get(last_array, 0), "alpha") == 0);
    assert(strcmp(*(char**)get(last_array, 4), "epsilon") == 0);
    
    char *new_word = strdup("modified");
    set(words1, 0, &new_word);
    assert(strcmp(*(char**)get(words1, 0), "modified") == 0);
    
    DynamicArray *check_array = *(DynamicArray**)get(arr_of_str_arrays, 0);
    assert(strcmp(*(char**)get(check_array, 0), "modified") == 0);
    
    destroy(arr_of_str_arrays);    
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("✅ (%.4f сек)\n", time_spent);
}


void test_mix_arays() {
    printf("Тест смешанного массива массивов -- ");
    clock_t start = clock();

    DynamicArray *arr_of_intstr_arrays = create(get_array_type());
    assert(arr_of_intstr_arrays != NULL);
    assert(get_size(arr_of_intstr_arrays) == 0);
    
    DynamicArray *words1 = create(get_string_type());
    char *s1_1 = strdup("apple");
    char *s1_2 = strdup("banana");
    char *s1_3 = strdup("cherry");
    push(words1, &s1_1);
    push(words1, &s1_2);
    push(words1, &s1_3);
    assert(get_size(words1) == 3);
    assert(strcmp(*(char**)get(words1, 0), "apple") == 0);
    
    DynamicArray *row1 = create(get_int_type());
    push(row1, &(int){1});
    push(row1, &(int){2});
    push(row1, &(int){3});
    assert(get_size(row1) == 3);
    assert(*(int*)get(row1, 0) == 1);
    
    push(arr_of_intstr_arrays, &words1);
    push(arr_of_intstr_arrays, &row1);
    assert(get_size(arr_of_intstr_arrays) == 2);
    
    assert(get_size(*(DynamicArray**)get(arr_of_intstr_arrays, 0)) == 3);
    assert(get_size(*(DynamicArray**)get(arr_of_intstr_arrays, 1)) == 3);
    
    DynamicArray *inner_str = *(DynamicArray**)get(arr_of_intstr_arrays, 0);
    DynamicArray *inner_int = *(DynamicArray**)get(arr_of_intstr_arrays, 1);
    
    assert(strcmp(*(char**)get(inner_str, 0), "apple") == 0);
    assert(*(int*)get(inner_int, 0) == 1);
    
    destroy(arr_of_intstr_arrays);    
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("✅ (%.4f сек)\n", time_spent);
}


void run_all_tests() {
    printf("========== ЗАПУСК ТЕСТОВ ==========\n");

    int i1 = 42, i2 = 7, i3 = 99, i4 = 1, i5 = 100;
    int *i_unsorted[] = {&i1, &i2, &i3, &i4, &i5};
    int *i_sorted[] = {&i4, &i2, &i1, &i3, &i5};

    test_core(get_int_type(), (void**)i_unsorted, 5, "int");
    test_algorithms(get_int_type(), (void**)i_unsorted, (void**)i_sorted, 5, int_map_square, int_is_even, "int");

    int d[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    void *ii[10];

    for (int k=0;k<10;k++) ii[k] = &d[k];
    test_stress(get_int_type(), ii, 10, "int");

    char *su1 = strdup("dog"), *su2 = strdup("cat"), *su3 = strdup("hamster"), *su4 = strdup("mouse"), *su5 = strdup("bird");
    char *s_vals[] = {su1, su2, su3, su4, su5};
    void *s_unsorted[5];

    for (int i = 0; i < 5; i++) s_unsorted[i] = &s_vals[i];

    void *s_sorted[5] = {&s_vals[4], &s_vals[1], &s_vals[0], &s_vals[2], &s_vals[3]};

    test_core(get_string_type(), s_unsorted, 5, "string");
    test_algorithms(get_string_type(), s_unsorted, s_sorted, 5, string_map_upper, string_length_gt_3, "string");

    char *swords[] = {"cat","dog","mouse","hamster","bird","fish","lion","tiger","bear","wolf"};
    void *ss[10];

    for (int i = 0; i < 10; i++) {
        swords[i] = strdup(swords[i]);
        ss[i] = &swords[i];
    }

    test_stress(get_string_type(), ss, 10, "string");

    for (int i = 0; i < 10; i++) free(swords[i]);

    test_int_arrays();
    test_string_arrays();
    test_mix_arays();

    free(su1); free(su2); free(su3); free(su4); free(su5);

    printf("\n============== Все тесты пройдены!✅ ==============\n");
}