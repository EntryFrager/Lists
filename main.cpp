#include "list.h"

int main ()
{
    LIST list = {};
    list_init (&list, 5);

    for (size_t ip = 1; ip <= 7; ip++)
    {
        list_insert_elem (&list, ip);
    }

    CHECK_ERROR_MAIN (list_delete_elem (&list, 3));

    CHECK_ERROR_MAIN (list_insert_elem_after_log (&list, 100, 0));

    CHECK_ERROR_MAIN (list_insert_elem_after_log (&list, 300, 3));

    CHECK_ERROR_MAIN (list_insert_elem_after_log (&list, 200, 5));

    CHECK_ERROR_MAIN (list_insert_elem_after_log (&list, 400, 9));

    CALL_DUMP (&list, 0);

    lineariz_list (&list);

    list_realloc (&list, DOWN);

    CALL_DUMP (&list, 0);

    list_print (&list);

    list_deinit (&list);

    return 0;
}