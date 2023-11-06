#include "list.h"

int main ()
{
    int value = 0;

    LIST *list_head = list_init (value);

    LIST *temp = list_head;

    for (size_t ip = 1; ip < 10; ip++)
    {
        temp = list_insert_elem (temp, ip);
    }

    list_print (list_head);

    list_head = list_move_elem (list_head->next, list_head->next->next->next, list_head);

    list_print (list_head);

    return 0;
}