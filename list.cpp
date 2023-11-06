#include "list.h"

LIST *list_init (int value)
{
    LIST *list = (LIST *) calloc (1, sizeof (LIST));
    my_assert (list != NULL);

    list->data = value;
    list->next = NULL;
    list->prev = NULL;

    return list;
}

LIST *list_insert_elem (LIST *list, int value)
{
    my_assert (list != NULL);

    LIST *after = (LIST *) calloc (1, sizeof (LIST));
    my_assert (after != NULL);

    LIST *next = list->next;

    after->data = value;
    after->next = list->next;
    after->prev = list;

    list->next = after;

    if (next != NULL)
    {
        next->prev = after;
    }

    return after;    
}

LIST *list_delete_elem (LIST *list)
{
    my_assert (list != NULL);

    LIST *prev = list->prev;
    LIST *next = list->next;

    if (prev != NULL)
    {
        prev->next = next;
    }

    if (next != NULL)
    {
        next->prev = prev;
    }

    free(list);
    list = NULL;

    return prev;
}

void list_print (LIST *list)
{
    LIST *temp = list;

    while (temp != NULL)
    {
        printf ("%d\n", temp->data);

        temp = temp->next;
    }
}

LIST *list_move_elem (LIST *list_first, LIST *list_second, LIST *head)
{
    LIST *prev_first = list_first->prev;
    LIST *next_first = list_first->next;
    LIST *prev_second = list_second->prev;
    LIST *next_second = list_second->next;

    if (next_first == list_second)
    {
        list_first->next = next_second;
        list_first->prev = list_second;

        list_second->next = list_first;
        list_second->prev = prev_first;

        if (list_first != head)
        {
            prev_first->next = list_second;
        }

        if (next_second != NULL)
        {
            next_second->prev = list_first;
        }
    }
    else if (next_second == list_first)
    {
        list_second->next = next_first;
        list_second->prev = list_first;

        list_first->next = list_second;
        list_first->prev = prev_second;

        if (list_second != head)
        {
            prev_second->next = list_first;
        }

        if (next_first != NULL)
        {
            next_first->prev = list_second;
        }
    }
    else
    {
        if (list_first != head)
        {
            prev_first->next = list_second;
        }
        if (list_second != head)
        {
            prev_second->next = list_first;
        }

        list_first->next  = next_second;
        list_second->next = next_first;
        list_first->prev  = prev_second;
        list_second->prev = prev_first;

        if (next_first != NULL)
        {
            next_first->prev = list_second;
        }
        if (next_second != NULL)
        {
            next_second->prev = list_first;
        }
    }

    if (list_first == head)
    {
        return list_second;
    }
    else if (list_second == head)
    {
        return list_first;
    }

    return head;
}