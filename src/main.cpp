#include "../include/list.h"

int main ()
{
    int code_error = 0;

    List list = {};
    list_init(&list, 5, &code_error);

    for (int ip = 1; ip <= 7; ip++)
    {
        list_insert_elem(&list, ip, &code_error);
    }

    list_delete_elem(&list, 3, &code_error);
    list_insert_elem_after_log(&list, 100, 0, &code_error);
    list_insert_elem_after_log(&list, 300, 3, &code_error);
    list_insert_elem_after_log(&list, 200, 5, &code_error);
    list_insert_elem_after_log(&list, 400, 9, &code_error);

    CALL_DUMP(&list, &code_error);

    lineariz_list(&list, &code_error);

    list_realloc(&list, DOWN, &code_error);

    CALL_DUMP(&list, &code_error);

    list_print(&list, &code_error);

    list_deinit(&list, &code_error);

    return 0;
}
