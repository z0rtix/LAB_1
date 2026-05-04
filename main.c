#include "TypeInfo/TypeInfo.h"
#include "UI_menu/menu.h"
#include "tests.h"

#include <stdio.h>


int main() {
    run_all_tests();
    interactive_mode();
    cleanup_types();
    
    return 0;
}
