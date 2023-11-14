#include "list.h"
// TD: readme
int main ()
{
    LIST list = {};
    list_init (&list, 5);

    for (size_t ip = 1; ip <= 7; ip++)
    {
        list_insert_elem (&list, ip);
    }

    CHECK_ERROR (list_delete_elem (&list, 3));

    list_dump_graph_viz (&list, 4, __FILE__, __func__, __LINE__); // TD: optional dump adter each func call

    CHECK_ERROR (list_insert_elem_after_log (&list, 100, 0));

    CHECK_ERROR (list_insert_elem_after_log (&list, 300, 3));

    CHECK_ERROR (list_insert_elem_after_log (&list, 200, 5));

    CHECK_ERROR (list_insert_elem_after_log (&list, 400, 9));

    list_print (&list);

    list_deinit (&list);

    return 0;
}