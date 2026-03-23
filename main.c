#include "Tests/tests_of_int.h"
#include "Tests/tests_of_str.h"
#include "TypeInfo/TypeInfo.h"
#include "UI_menu/menu.h"

#include <stdio.h>


int main() {
    tests_of_int();
    tests_of_str();
    test_int_arrays();
    test_string_arrays();
    test_mix_arays();

    printf("\n============== Все тесты пройдены!✅ ==============\n");

    interactive_mode();
    cleanup_types();
    
    return 0;
}
