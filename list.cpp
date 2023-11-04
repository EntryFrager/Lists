#include "list.h"

void list_ctor (LIST *list, size_t size)
{
    my_assert (list != NULL);
    my_assert (size > 0);

    list->buf = (ELEMENT *) calloc (size + 1, sizeof (ELEMENT));
    my_assert (list->buf);
    list->buf[0] = VALUE_VENOM;

    list->next = (size_t *) calloc (size + 1, sizeof (size_t));
    my_assert (list->next != NULL);

    list->prev = (size_t *) calloc (size + 1, sizeof (size_t));
    my_assert (list->prev != NULL);

    list->head = 1;
    list->tale = 1;
    list->free = size;
}

void list_dtor (LIST *list)
{
    my_assert (list != NULL);
}