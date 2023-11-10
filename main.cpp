#include "list.h"

int main ()
{
    LIST *list = list_init (5);

    for (size_t ip = 1; ip <= 7; ip++)
    {
        list_insert_elem (list, ip);
    }

    list_delete_elem (list, list->tail);

    list_insert_elem_after (list, 100, 0);

    list_insert_elem_after (list, 300, 3);

    list_insert_elem_after (list, 200, 5);

    list_insert_elem_after (list, 400, 9);

    printf ("%d", list_get_elem_ip (list, 11));

    lineariz_list (list);

    printf ("%d", list_get_elem_ip (list, 11));

    list_dump (list, 4, __FILE__, __func__, __LINE__);

    list_print (list);

    list_deinit (list);

    return 0;
}