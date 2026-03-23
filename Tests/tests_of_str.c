#include "tests_of_str.h"
#include "../utils/utils.h"
#include "../TypeInfo/TypeInfo.h"
#include "../DynamicArray/DynamicArray.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>


void test_string_create_destroy() {
    printf("Тест create/destroy -- ");
    DynamicArray *arr = create(get_string_type());

    assert(arr != NULL);
    assert(get_size(arr) == 0);
    assert(is_empty(arr) == 1);
    
    destroy(arr);
    printf("✅\n");
}


void test_string_clear() {
    printf("Тест clear -- ");
    DynamicArray *arr = create(get_string_type());
    char *s1 = strdup("кот");
    char *s2 = strdup("собака");
    char *s3 = strdup("хомяк");
    push(arr, &s1);
    push(arr, &s2);
    push(arr, &s3);
    assert(get_size(arr) == 3);
    
    clear(arr);
    assert(get_size(arr) == 0);
    assert(get_capacity(arr) == DEFAULT_CAPACITY);
    assert(arr->data != NULL);
    
    destroy(arr);
    printf("✅\n");
}

void test_string_push_get() {
    printf("Тест push/get -- ");
    DynamicArray *arr = create(get_string_type());
    char *s1 = strdup("cat");
    char *s2 = strdup("dog");
    char *s3 = strdup("mouse");
    push(arr, &s1);
    push(arr, &s2);
    push(arr, &s3);
    
    assert(get_size(arr) == 3);
    assert(strcmp(*(char**)get(arr, 0), "cat") == 0);
    assert(strcmp(*(char**)get(arr, 1), "dog") == 0);
    assert(strcmp(*(char**)get(arr, 2), "mouse") == 0);
    assert(get(arr, 5) == NULL);
    
    destroy(arr);
    printf("✅\n");
}

void test_string_pop() {
    printf("Тест pop -- ");
    DynamicArray *arr = create(get_string_type());
    char *s1 = strdup("кот");
    char *s2 = strdup("собака");
    char *s3 = strdup("хомяк");
    push(arr, &s1);
    push(arr, &s2);
    push(arr, &s3);
    assert(get_size(arr) == 3);
    assert(strcmp(*(char**)get(arr, 2), "хомяк") == 0);
    
    pop(arr);
    assert(get_size(arr) == 2);
    assert(strcmp(*(char**)get(arr, 1), "собака") == 0);
    
    pop(arr);
    assert(get_size(arr) == 1);
    assert(strcmp(*(char**)get(arr, 0), "кот") == 0);
    
    pop(arr);
    assert(get_size(arr) == 0);
    
    pop(arr); 
    assert(get_size(arr) == 0);
    
    destroy(arr);
    printf("✅\n");
}

void test_string_remove_at() {
    printf("Тест remove_at -- ");
    DynamicArray *arr = create(get_string_type());
    char *words[] = {"один", "два", "три", "четыре", "пять"};
    char *copies[5];
    for (int i = 0; i < 5; i++) {
        copies[i] = strdup(words[i]);
        push(arr, &copies[i]);
    }
    assert(get_size(arr) == 5);
    
    remove_at(arr, 2);    
    assert(get_size(arr) == 4);
    assert(strcmp(*(char**)get(arr, 0), "один") == 0);
    assert(strcmp(*(char**)get(arr, 1), "два") == 0);
    assert(strcmp(*(char**)get(arr, 2), "четыре") == 0);
    assert(strcmp(*(char**)get(arr, 3), "пять") == 0);

    remove_at(arr, 0);
    assert(get_size(arr) == 3);
    assert(strcmp(*(char**)get(arr, 0), "два") == 0);
    
    remove_at(arr, 2);
    assert(get_size(arr) == 2);
    assert(strcmp(*(char**)get(arr, 1), "четыре") == 0);
    
    destroy(arr);
    printf("✅\n");
}

void test_string_set() {
    printf("Тест set -- ");
    DynamicArray *arr = create(get_string_type());
    char *s1 = strdup("cat");
    char *s2 = strdup("dog");
    push(arr, &s1);
    push(arr, &s2);
    char *new_str = strdup("hamster");
    set(arr, 0, &new_str);
    
    assert(strcmp(*(char**)get(arr, 0), "hamster") == 0);
    assert(strcmp(*(char**)get(arr, 1), "dog") == 0);
    
    destroy(arr);
    printf("✅\n");
}

void test_string_reserve() {
    printf("Тест reserve -- ");
    DynamicArray *arr = create(get_string_type());
    int initial_get_capacity = get_capacity(arr);    
    reserve(arr, initial_get_capacity + 10);
    assert(get_capacity(arr) >= initial_get_capacity + 10);
    assert(get_size(arr) == 0);
    
    char *s = strdup("тест");
    push(arr, &s);
    assert(get_capacity(arr) >= initial_get_capacity + 10);
    assert(get_size(arr) == 1);
    
    reserve(arr, 1);
    assert(get_capacity(arr) >= initial_get_capacity + 10);
    
    destroy(arr);
    printf("✅\n");
}

void test_string_shrink_to_fit() {
    printf("Тест shrink_to_fit -- ");
    DynamicArray *arr = create(get_string_type());
    for (int i = 0; i < 20; i++) {
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "str_%d", i);

        char *s = strdup(buffer);
        push(arr, &s);
    }
    int before_get_capacity = get_capacity(arr);
    assert(before_get_capacity > 20);
    
    for (int i = 0; i < 10; i++) {
        pop(arr);
    }
    assert(get_size(arr) == 10);
    
    shrink_to_fit(arr);    
    assert(get_capacity(arr) == 10);
    assert(get_size(arr) == 10);
    
    for (int i = 0; i < 10; i++) {
        char expected[20];
        snprintf(expected, sizeof(expected), "str_%d", i);
        char *actual = *(char**)get(arr, i);
        assert(strcmp(actual, expected) == 0);
    }
    
    destroy(arr);
    printf("✅\n");
}

void test_string_sort() {
    printf("Тест sort -- ");
    DynamicArray *arr = create(get_string_type());
    char *s1 = strdup("dog");
    char *s2 = strdup("cat");
    char *s3 = strdup("hamster");
    char *s4 = strdup("mouse");
    push(arr, &s1);
    push(arr, &s2);
    push(arr, &s3);
    push(arr, &s4);
    sort(arr);
    
    assert(strcmp(*(char**)get(arr, 0), "cat") == 0);
    assert(strcmp(*(char**)get(arr, 1), "dog") == 0);
    assert(strcmp(*(char**)get(arr, 2), "hamster") == 0);
    assert(strcmp(*(char**)get(arr, 3), "mouse") == 0);
    
    destroy(arr);
    printf("✅\n");
}

void test_string_copy() {
    printf("Тест copy -- ");
    DynamicArray *original = create(get_string_type());
    char *s1 = strdup("кот");
    char *s2 = strdup("собака");
    char *s3 = strdup("хомяк");
    push(original, &s1);
    push(original, &s2);
    push(original, &s3);
    DynamicArray *clone = copy(original);
    
    assert(get_size(original) == get_size(clone));
    
    for (int i = 0; i < get_size(original); i++) {
        char *orig_str = *(char**)get(original, i);
        char *clone_str = *(char**)get(clone, i);
        assert(strcmp(orig_str, clone_str) == 0);
        assert(orig_str != clone_str);
    }
    
    destroy(original);
    destroy(clone);
    printf("✅\n");
}

void test_string_map() {
    printf("Тест map -- ");
    DynamicArray *arr = create(get_string_type());
    char *s1 = strdup("cat");
    char *s2 = strdup("dog");
    char *s3 = strdup("mouse");
    push(arr, &s1);
    push(arr, &s2);
    push(arr, &s3);
    DynamicArray *upper = map(arr, string_map_upper);
    // print_array(arr);
    // print_array(upper);
    
    assert(get_size(upper) == 3);
    assert(strcmp(*(char**)get(upper, 0), "CAT") == 0);
    assert(strcmp(*(char**)get(upper, 1), "DOG") == 0);
    assert(strcmp(*(char**)get(upper, 2), "MOUSE") == 0);
    
    destroy(upper);
    destroy(arr);
    printf("✅\n");
}

void test_string_where() {
    printf("Тест where -- ");
    DynamicArray *arr = create(get_string_type());
    char *s1 = strdup("cat");
    char *s2 = strdup("dog");
    char *s3 = strdup("mouse");
    char *s4 = strdup("hamster");
    // char *s5 = strdup("hamster2");
    push(arr, &s1);
    push(arr, &s2);
    push(arr, &s3);
    push(arr, &s4);
    DynamicArray *long_words = where(arr, string_length_gt_3);
    
    assert(get_size(long_words) == 2);
    assert(strcmp(*(char**)get(long_words, 0), "mouse") == 0);
    assert(strcmp(*(char**)get(long_words, 1), "hamster") == 0);

    destroy(long_words);
    destroy(arr);
    printf("✅\n");
}

void test_string_where_starts_with_a() {
    printf("Тест where starts with A ");
    DynamicArray *arr = create(get_string_type());
    char *s1 = strdup("apple");
    char *s2 = strdup("banana");
    char *s3 = strdup("apricot");
    char *s4 = strdup("orange");
    push(arr, &s1);
    push(arr, &s2);
    push(arr, &s3);
    push(arr, &s4);
    DynamicArray *a_words = where(arr, string_starts_with_a);
    
    assert(get_size(a_words) == 2);
    assert(strcmp(*(char**)get(a_words, 0), "apple") == 0);
    assert(strcmp(*(char**)get(a_words, 1), "apricot") == 0);

    destroy(a_words);
    destroy(arr);
    printf("✅\n");
}

void test_string_concat() {
    printf("Тест concat -- ");
    DynamicArray *arr1 = create(get_string_type());
    DynamicArray *arr2 = create(get_string_type());
    char *a1 = strdup("cat");
    char *a2 = strdup("dog");
    char *b1 = strdup("mouse");
    char *b2 = strdup("hamster");
    push(arr1, &a1);
    push(arr1, &a2);
    push(arr2, &b1);
    push(arr2, &b2);
    
    DynamicArray *arr3 = concat(arr1, arr2);
    assert(get_size(arr3) == 4);
    assert(strcmp(*(char**)get(arr3, 0), "cat") == 0);
    assert(strcmp(*(char**)get(arr3, 1), "dog") == 0);
    assert(strcmp(*(char**)get(arr3, 2), "mouse") == 0);
    assert(strcmp(*(char**)get(arr3, 3), "hamster") == 0);

    destroy(arr1);
    destroy(arr2);
    destroy(arr3);
    printf("✅\n");
}

void test_string_boundaries() {
    printf("Тест граничных случаев -- ");    
    DynamicArray *arr = create(get_string_type());
    assert(arr != NULL);
    assert(get_size(arr) == 0);
    assert(is_empty(arr) == 1);
    assert(get(arr, 0) == NULL);

    pop(arr);
    assert(get_size(arr) == 0);
    
    remove_at(arr, 0);
    assert(get_size(arr) == 0);
    
    char *s1 = strdup("первый");
    push(arr, &s1);
    assert(get_size(arr) == 1);
    assert(strcmp(*(char**)get(arr, 0), "первый") == 0);
    
    assert(get(arr, -1) == NULL);
    assert(get(arr, 1) == NULL);
    
    char *new_str = strdup("новый");
    set(arr, -1, &new_str);
    assert(strcmp(*(char**)get(arr, 0), "первый") == 0);
    free(new_str);
    
    int current_get_capacity = get_capacity(arr);
    for (int i = 1; i < current_get_capacity; i++) {
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "str_%d", i);
        char *s = strdup(buffer);
        push(arr, &s);
    }
    assert(get_size(arr) == current_get_capacity);
    
    char *last = strdup("последний");
    push(arr, &last);
    assert(get_size(arr) == current_get_capacity + 1);
    
    pop(arr);
    assert(get_size(arr) == current_get_capacity);
    
    clear(arr);
    assert(get_size(arr) == 0);
    assert(get_capacity(arr) == DEFAULT_CAPACITY);
    
    destroy(arr);
    printf("✅\n");
}

void test_string_10m_operations() {
    printf("Тест 10m operations -- ");
    clock_t start = clock();
    DynamicArray *arr = create(get_string_type());
    reserve(arr, 10000000);

    char *words[] = {"cat", "dog", "mouse", "hamster", "bird", "fish", "lion", "tiger", "bear", "wolf"};
    
    for (int i = 0; i < 10000000; i++) {
        char *copy = strdup(words[i % 10]);
        push(arr, &copy);
    }

    destroy(arr);
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

void tests_of_str() {
    printf("\n========== Тесты STRING ==========\n\n");
    clock_t start = clock();
    
    test_string_create_destroy();
    test_string_clear();
    test_string_push_get();
    test_string_pop();
    test_string_remove_at();
    test_string_set();
    test_string_reserve();
    test_string_shrink_to_fit();
    test_string_sort();
    test_string_copy();
    test_string_map();
    test_string_where();
    // test_string_where_starts_with_a();
    test_string_concat();
    test_string_boundaries();
    test_string_10m_operations();
    
    printf("\n✅ ✅ ✅ ТЕСТЫ STRING ПРОЙДЕНЫ ✅ ✅ ✅");
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("  (%.5f сек)\n\n", time_spent);
}