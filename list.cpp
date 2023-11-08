#include "list.h"

const char *err_msgs_arr[] = {
    "ERROR NO.\n",
    "ERROR: null pointer to list.\n",
    "ERROR: incorrect list size.\n",
    "ERROR: invalid pointer to the head of the list.\n",
    "ERROR: invalid pointer to the tail of the list.\n",
    "ERROR: invalid pointer to the first free cell in the list.\n",
    "ERROR: invalid pointer to data array.\n",
    "ERROR: invalid pointer to an array with pointers to the following elements.\n",
    "ERROR: invalid pointer to an array with pointers to previous elements.\n",
    "ERROR: your list is full of shit, call init.\n",
    "ERROR: error when opening file.\n",
    "ERROR: error when closing file.\n",
};

static const int HST_UP = 2;

static const char *fp_err_name = "file_err.txt";

LIST *list_init (size_t size)
{    
    LIST *list = (LIST *) calloc (1, sizeof (LIST));
    my_assert (list != NULL);

    list->data = (int *) calloc ((size + 1) * sizeof (int), sizeof (char));
    my_assert (list->data != NULL);

    list->next = (int *) calloc ((size + 1) * sizeof (int), sizeof (char));
    my_assert (list->next != NULL);

    for (size_t ip = 1; ip < size + 1; ip++)
    {
        list->next[ip] = ip + 1;
    }

    list->prev = (int *) calloc ((size + 1) * sizeof (int), sizeof (char));
    my_assert (list->prev != NULL);
    memset (list->prev + 1, PREV_NO_ELEM, size * sizeof (int));

    list->size = size + 1;
    list->head = 1;
    list->tail = 0;
    list->free = 1;

    assert_list (list);

    return list;
}

void list_insert_elem (LIST *list, int value)
{
    assert_list (list);

    if (list->free == (list->size - 1))
    {
        list_realloc (list);
    }

    list->data[list->free] = value;
    
    list->prev[list->free] = list->tail;

    list->next[list->free] = 0;
    list->next[list->tail] = list->free;
    
    list->tail = list->free;

    for (int ip = 1; ip < list->size; ip++)
    {
        if (list->prev[ip] == PREV_NO_ELEM)
        {
            list->free = ip;
            break;
        }
    }
    
    assert_list (list);
}

int list_delete_elem (LIST *list, int ip)
{
    assert_list (list);

    if (list->prev[ip] == PREV_NO_ELEM || ip > list->size)
    {
        return ERR_DELETE;
    }

    if (ip == list->head)
    {
        list->head = list->next[ip];
    }

    list->next[list->prev[ip]] = list->next[ip];
    list->prev[list->next[ip]] = list->prev[ip];

    list->prev[ip] = PREV_NO_ELEM;
    list->next[ip] = list->free;
    list->free = ip;

    assert_list (list);

    return ERR_NO;
}

void list_realloc (LIST *list)
{
    assert_list (list);

    int size = list->size * HST_UP;

    list->data = (int *) realloc (list->data, size * sizeof (int));
    my_assert (list->data != NULL);
    memset (list->data + list->size, 0, sizeof (int) * (size - list->size));

    list->next = (int *) realloc (list->next, size * sizeof (int));
    my_assert (list->next != NULL);
    memset (list->next + list->size, 0, sizeof (int) * (size - list->size));

    list->prev = (int *) realloc (list->prev, size * sizeof (int));
    my_assert (list->prev != NULL);
    memset (list->prev + list->size, -1, sizeof (int) * (size - list->size));

    list->size = size;

    assert_list (list);
}

void list_print (LIST *list)
{
    assert_list (list);

    int ip = list->head;

    while (list->next[ip] != 0)
    {
        printf ("%d\n", list->data[ip]);

        ip = list->next[ip];
    }

    printf ("%d\n", list->data[ip]);
}

void list_deinit (LIST *list)
{
    assert_list (list);

    free (list->data);
    free (list->next);
    free (list->prev);

    list->data = NULL;
    list->next = NULL;
    list->prev = NULL;

    list->size = LIST_VALUE_VENOM;
    list->head = LIST_VALUE_VENOM;
    list->tail = LIST_VALUE_VENOM;
    list->free = LIST_VALUE_VENOM;

    assert_list (list);
}

int list_verification (LIST *list)
{
    if (list == NULL)
    {
        return LIST_ERR;
    }

    if (list->size == LIST_VALUE_VENOM && list->head == LIST_VALUE_VENOM && list->tail == LIST_VALUE_VENOM && list->free == LIST_VALUE_VENOM)
    {
        return LIST_POINTER_GARBAGE;
    }

    if (list->size <= 0)
    {
        return LIST_ERR_SIZE;
    }

    if (list->data == NULL)
    {
        return LIST_ERR_DATA;
    }

    if (list->next == NULL)
    {
        return LIST_ERR_NEXT;
    }

    if (list->prev == NULL)
    {
        return LIST_ERR_PREV;
    }

    if (list->head < 0 || list->head >= list->size)
    {
        return LIST_ERR_HEAD;
    }

    if (list->tail < 0 || list->tail >= list->size)
    {
        return LIST_ERR_TAIL;
    }

    if (list->free < 0 || list->free >= list->size || list->prev[list->free] != -1)
    {
        return LIST_ERR_FREE;
    }

    return LIST_OK;
}

void list_dump (LIST *list, const int code_error, const char *file_err, const char *func_err, const int line_err)
{
    FILE *fp_err = fopen (fp_err_name, "a");

    if (fp_err == NULL)
    {
        fprintf (stderr, "%s", err_msgs_arr[FILE_OPEN_ERR]);
    }
    
    if (list != NULL)
    {
        if (code_error < LIST_ERROR_CNT)
        {
            fprintf (fp_err, "%s\n", err_msgs_arr[code_error]);
        }
        else
        {
            fprintf (fp_err, "Unknown error.\n\n");
        }

        fprintf (fp_err, "list[%p] \"list\" called from %s(%d) %s\n", list, file_err, line_err, func_err);

        fprintf (fp_err, "{\n");

        fprintf (fp_err, "\tsize = %d\n", list->size);
        fprintf (fp_err, "\thead = %d\n", list->head);
        fprintf (fp_err, "\ttail = %d\n", list->tail);
        fprintf (fp_err, "\tfree = %d\n", list->free);

        if (list->data != NULL)
        {
            fprintf (fp_err, "\tdata[%p]\n", list->data);
            fprintf (fp_err, "\t{\n");

            if (list->size > 0)
            {
                for (int i = 0; i < list->size; i++)
                {
                    fprintf (fp_err, "\t\t*[%d] = %d\n", i, list->data[i]);
                }
            }
            fprintf (fp_err, "\t}\n");
        }
        else 
        {
            fprintf (fp_err, "\tdata[NULL]\n");
        }

        if (list->next != NULL)
        {
            fprintf (fp_err, "\tnext[%p]\n", list->next);
            fprintf (fp_err, "\t{\n");

            if (list->size > 0)
            {
                for (int i = 0; i < list->size; i++)
                {
                    fprintf (fp_err, "\t\t*[%d] = %d\n", i, list->next[i]);
                }
            }
            fprintf (fp_err, "\t}\n");
        }
        else 
        {
            fprintf (fp_err, "\tnext[NULL]\n");
        }

        if (list->prev != NULL)
        {
            fprintf (fp_err, "\tprev[%p]\n", list->prev);
            fprintf (fp_err, "\t{\n");

            if (list->size > 0)
            {
                for (int i = 0; i < list->size; i++)
                {
                    fprintf (fp_err, "\t\t*[%d] = %d\n", i, list->prev[i]);
                }
            }
            fprintf (fp_err, "\t}\n");
        }
        else 
        {
            fprintf (fp_err, "\tprev[NULL]\n");
        }

        fprintf (fp_err, "}\n\n-----------------------------------------------------------\n");

        if (fclose (fp_err) != 0)
        {
            fprintf (fp_err, "%s", err_msgs_arr[FILE_CLOSE_ERR]);
        }
    }
    else
    {
        if (code_error < ERROR_CNT)
        {
            fprintf (fp_err, "%s", err_msgs_arr[code_error]);
        }
        else
        {
            fprintf (fp_err, "Unknown error.\n\n");
        }

        fprintf (fp_err, "list[NULL] \"list\" called from %s(%d) %s\n", file_err, line_err, func_err);
    }
}