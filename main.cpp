#include "list.h"

int main ()
{
    LIST list = {};

    list_init (&list, 5);
    
    for (size_t ip = 1; ip <= 7; ip++)
    {
        list_insert_elem (&list, ip);
    }

    CHECK_ERROR (list_delete_elem (&list, list.tail));

    CHECK_ERROR (list_insert_elem_after (&list, 100, 0));

    CHECK_ERROR (list_insert_elem_after (&list, 300, 3));

    CHECK_ERROR (list_insert_elem_after (&list, 200, 5));

    CHECK_ERROR (list_insert_elem_after (&list, 400, 9));

    lineariz_list (&list);
    
    list_move_elem (&list, 1, 10);

    lineariz_list (&list);

    list_dump_graph_viz (&list, 4, __FILE__, __func__, __LINE__);

    list_print (&list);

    list_deinit (&list);

    return 0;
}