#include "list.h"

#define WHITE_COLOR "\"#ffffff\""
#define TURQUOISE_COLOR "\"#40E0D0\""
#define PURPLE_COLOR "\"#8B00FF\""
#define RED_COLOR "\"#ff0000\""
#define LIGHT_GREEN_COLOR "\"#ccff99\""
#define BACK_GROUND_COLOR "\"#B8860B\""

const char *err_msgs_arr[] = {
    "ERROR NO.",
    "ERROR: null pointer to list.",
    "ERROR: incorrect list size.",
    "ERROR: invalid pointer to the head of the list.",
    "ERROR: invalid pointer to the tail of the list.",
    "ERROR: invalid pointer to the first free cell in the list.",
    "ERROR: invalid pointer to data array.",
    "ERROR: the connection between the list nodes is broken.",
    "ERROR: the connection between free nodes of the list is broken.",
    "ERROR: the number of nodes is incorrect.",
    "ERROR: your list is full of shit, call init.",
    "ERROR: error when opening file.",
    "ERROR: error when closing file.",
};

static const int HST_UP = 2;

static const char *fp_err_name = "file_err.txt";
static const char *fp_dot_name = "dump.dot";

LIST *list_init (const size_t size)
{
    my_assert (size > 0);
    LIST *list = (LIST *) calloc (1, sizeof (LIST));
    my_assert (list != NULL);

    list->data = (NODE *) calloc ((size + 1), sizeof (NODE));
    my_assert (list->data != NULL);

    for (size_t ip = 0; ip < size + 1; ip++)
    {
        if (ip == size)
        {
            list->data[ip].next = 0;
        }
        else
        {
            list->data[ip].next = ip + 1;
        }

        list->data[ip].prev = PREV_NO_ELEM;
    }

    list->size = size + 1;
    list->head = 1;
    list->tail = 0;
    list->free = 1;

    assert_list (list);

    return list;
}

void list_insert_elem (LIST *list, const int value)
{
    assert_list (list);

    if (list->free == (list->size - 1))
    {
        list_realloc (list);
    }

    list->data[list->free].value = value;
    list->data[list->free].prev  = list->tail;

    int free_next = list->data[list->free].next;
    list->data[list->free].next = 0;

    list->data[0].prev = list->free;

    list->data[list->tail].next = list->free;

    list->tail = list->free;
    list->free = free_next;
    
    assert_list (list);
}

int list_insert_elem_after (LIST *list, const int value, int ip)
{
    assert_list (list);

    if (ip < 0 || ip > list->size)
    {
        return ERR_INSERT;
    }

    if (list->free == (list->size - 1))
    {
        list_realloc (list);
    }

    ip = list_get_elem_ip (list, ip);

    if (ip == list->tail)
    {
        list_insert_elem (list, value);
    }
    else
    {
        if (ip == 0)
        {
            list->head = list->free;
        }

        int free_next = list->data[list->free].next;

        list->data[list->free].value = value;
        list->data[list->free].next = list->data[ip].next;
        list->data[list->free].prev = ip;

        list->data[list->data[ip].next].prev = list->free;
        list->data[ip].next = list->free;

        list->free = free_next;
    }

    assert_list (list);

    return ERR_NO;
}

int list_delete_elem (LIST *list, int ip)
{
    assert_list (list);

    if (list->data[ip].prev == PREV_NO_ELEM || ip > list->size)
    {
        return ERR_DELETE;
    }

    ip = list_get_elem_ip (list, ip);

    if (ip == list->head)
    {
        list->head = list->data[list->head].next;
    }
    else if (ip == list->tail)
    {
        list->tail = list->data[list->tail].prev;
    }

    list->data[list->data[ip].next].prev = list->data[ip].prev;
    list->data[list->data[ip].prev].next = list->data[ip].next;

    list->data[ip].prev = PREV_NO_ELEM;
    list->data[ip].next = list->free;
    list->free = ip;

    assert_list (list);

    return ERR_NO;
}

int list_get_elem_ip (LIST *list, int ip)
{
    assert_list (list);
    my_assert (ip >= 0 && ip < list->size);

    int ip_pos = list->head;

    if (ip == 0)
    {
        ip_pos = ip;
    }
    else
    {
        for (int counter = 0; counter < ip - 1; counter++)
        {
            ip_pos = list->data[ip_pos].next;
        }
    }

    return ip_pos;
}

void list_realloc (LIST *list)
{
    assert_list (list);

    int last_free = list->free;

    while (list->data[last_free].next != 0)
    {
        last_free = list->data[last_free].next;
    }

    int size = list->size * HST_UP;

    list->data = (NODE *) realloc (list->data, size * sizeof (NODE));
    my_assert (list->data != NULL);

    list->data[last_free].next = list->size;

    for (int ip = list->size - 1; ip < size; ip++)
    {
        if (ip == size - 1)
        {
            list->data[ip].next = 0;
        }
        else
        {
            list->data[ip].next = ip + 1;
        }

        list->data[ip].value = 0;
        list->data[ip].prev  = -1;
    }

    list->size = size;

    assert_list (list);
}

void lineariz_list (LIST *list)
{
    assert_list (list);

    NODE *new_data = (NODE *) calloc (list->size, sizeof (NODE));

    new_data[0].next = 1;

    int ip = list->head;
    int counter = 1;
    list->head = 1;

    do {
        new_data[counter].value = list->data[ip].value;
        new_data[counter].next  = counter + 1;
        new_data[counter].prev  = counter - 1;

        ip = list->data[ip].next;
        counter++;
    } while (ip != 0);

    new_data[counter - 1].next = 0;
    list->tail = counter - 1;
    new_data[0].prev = list->tail;
    ip = list->free;
    list->free = counter;

    do {
        new_data[counter].next = counter + 1;
        new_data[counter].prev = -1;

        ip = list->data[ip].next;
        counter++;
    } while (ip != 0);

    new_data[counter - 1].next = 0;

    list->data = new_data;

    assert_list (list);
}

void list_print (LIST *list)
{
    assert_list (list);

    int ip = list->head;

    while (ip != 0)
    {
        printf ("%d\n", list->data[ip].value);

        ip = list->data[ip].next;
    }
}

void list_deinit (LIST *list)
{
    assert_list (list);

    free (list->data);

    list->data = NULL;

    list->size = LIST_VALUE_VENOM;
    list->head = LIST_VALUE_VENOM;
    list->tail = LIST_VALUE_VENOM;
    list->free = LIST_VALUE_VENOM;
}

#ifdef DEBUG

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

    if (list->head < 0 || list->head >= list->size)
    {
        return LIST_ERR_HEAD;
    }

    if (list->tail < 0 || list->tail >= list->size)
    {
        return LIST_ERR_TAIL;
    }

    if (list->free < 0 || list->free >= list->size || (list->data[list->free].prev != -1))
    {
        return LIST_ERR_FREE;
    }

    int ip = 0;
    int counter = 0;

    do {
        if (list->data[list->data[ip].next].prev == -1)
        {
            counter++;
            break;
        }

        if (list->data[list->data[ip].next].prev != ip)
        {
            return LIST_ERR_PTR_DATA;
        }

        ip = list->data[ip].next;
        counter++;
    } while (ip != 0);


    if (list->free > 0)
    {
        ip = list->free;
        
        do {
            if (list->data[ip].prev != PREV_NO_ELEM)
            {
                return LIST_ERR_PTR_FREE;
            }

            ip = list->data[ip].next;
            counter++;
        } while (ip != 0);
    }

    if (counter != list->size)
    {
        return LIST_ERR_CNT_NODE;
    }

    return LIST_OK;
}

void list_dump_text (LIST *list, const int code_error, const char *file_err, const char *func_err, const int line_err)
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
            fprintf (fp_err, "%s\n\n", err_msgs_arr[code_error]);
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

            if (code_error != LIST_ERR_SIZE)
            {
                for (int ip = 0; ip < list->size; ip++)
                {
                    fprintf (fp_err, "\t\tdata[%d]->value = %d\n", ip, list->data[ip].value);
                    fprintf (fp_err, "\t\tdata[%d]->next  = %d\n", ip, list->data[ip].next);
                    fprintf (fp_err, "\t\tdata[%d]->prev  = %d\n", ip, list->data[ip].prev);
                }
            }
            
            fprintf (fp_err, "\t}\n");
        }
        else
        {
            fprintf (fp_err, "\tdata[NULL]\n");
        }

        fprintf (fp_err, "}\n\n-----------------------------------------------------------\n");
    }
    else
    {
        if (code_error < ERROR_CNT)
        {
            fprintf (fp_err, "%s\n\n", err_msgs_arr[code_error]);
        }
        else
        {
            fprintf (fp_err, "Unknown error.\n\n");
        }

        fprintf (fp_err, "list[NULL] \"list\" called from %s(%d) %s\n", file_err, line_err, func_err);
    }

    if (fclose (fp_err) != 0)
    {
        fprintf (stderr, "%s", err_msgs_arr[FILE_CLOSE_ERR]);
    }
}

void list_dump_graph_viz (LIST *list, const int code_error, const char *file_err, const char *func_err, const int line_err)
{
    FILE *fp_dot  = fopen (fp_dot_name, "wb+");

    if (fp_dot == NULL)
    {
        fprintf (stderr, "%s", err_msgs_arr[FILE_OPEN_ERR]);
    }

    int weight = 10000;
    const char *color = WHITE_COLOR;

    if (list != NULL)
    {
        fprintf (fp_dot, "digraph List {\n");
        fprintf (fp_dot, "\trankdir = LR;\n");
        fprintf (fp_dot, "\tnode [shape = record];\n");
        fprintf (fp_dot, "\tbgcolor = " BACK_GROUND_COLOR ";\n");

        if (code_error != LIST_ERR_SIZE)
        {
            fprintf (fp_dot, "\t0 -> 0");

            for (int ip = 1; ip < list->size; ip++)
            {
                fprintf (fp_dot, " -> %d", ip);
            }

            fprintf (fp_dot, " -> head -> tail -> free[arrowsize = 0.0, weight = %d, color = " BACK_GROUND_COLOR "];\n", weight);

            for (int ip = 0; ip < list->size; ip++)
            {
                if (list->data[ip].prev != PREV_NO_ELEM && ip != 0)
                {
                    color = LIGHT_GREEN_COLOR;
                }
                else if (ip != 0)
                {
                    color = TURQUOISE_COLOR;
                }
                else
                {
                    color = PURPLE_COLOR;
                }

                fprintf (fp_dot, "\t%d [shape = Mrecord, style = filled, fillcolor = %s, label = \"idx: %d | data: %d | next: %d | prev: %d\"];\n", 
                            ip, color, ip, list->data[ip].value, list->data[ip].next, list->data[ip].prev);
            }
        }

        fprintf (fp_dot, "\thead [shape = Mrecord, style = filled, fillcolor = " LIGHT_GREEN_COLOR ", label = \"head: %d\"];\n", list->head);
        fprintf (fp_dot, "\ttail [shape = Mrecord, style = filled, fillcolor = " LIGHT_GREEN_COLOR ", label = \"tail: %d\"];\n", list->tail);
        fprintf (fp_dot, "\tfree [shape = Mrecord, style = filled, fillcolor = " TURQUOISE_COLOR ", label = \"free: %d\"];\n", list->free);

        int ip = 0;
        
        do
        {
            fprintf (fp_dot, "\t%d -> %d\n", ip, list->data[ip].next);

            ip = list->data[ip].next;
        } while (ip != 0);

        if (code_error != LIST_ERR_FREE)
        {
            ip = list->free;

            while (ip != 0)
            {
                fprintf (fp_dot, "\t%d -> %d\n", ip, list->data[ip].next);

                ip = list->data[ip].next;
            }
        }

        fprintf (fp_dot, "\thead -> %d\n", list->head);
        fprintf (fp_dot, "\ttail -> %d\n", list->tail);
        fprintf (fp_dot, "\tfree -> %d\n", list->free);

        fprintf (fp_dot, "\tlabel = \"list_dump from function %s, List/%s:%d\";\n", func_err, file_err, line_err);
        fprintf (fp_dot, "\tAll[shape = Mrecord, style = filled, fillcolor = " RED_COLOR ", label = \"size = %d | data_ptr = %p\"];}\n", list->size, list->data);
    }

    if (fclose (fp_dot) != 0)
    {
        fprintf (stderr, "%s", err_msgs_arr[FILE_CLOSE_ERR]);
    }

    system ("dot -Tsvg dump.dot -o dot.svg");
}

#endif