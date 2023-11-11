#include "list.h"

#define WHITE_COLOR "\"#ffffff\""
#define BLUE_COLOR "\"#00BFFF\""
#define YELLOW_COLOR "\"#FFFF00\""
#define PURPLE_COLOR "\"#8B00FF\""
#define RED_COLOR "\"#ff0000\""
#define LIGHT_GREEN_COLOR "\"#ccff99\""
#define BACK_GROUND_COLOR "\"#696969\""

#define FONTNAME "\"Times-New-Roman\""

#define CHECK_SIZE() if (list->free == (list->size - 1)) {list_realloc (list);}

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

LIST *list_init (LIST *list, const size_t size)
{
    my_assert (size > 0);
    my_assert (list != NULL);

    list->data = (NODE *) calloc (size, sizeof (NODE));
    my_assert (list->data != NULL);

    for (size_t ip = 0; ip < size; ip++)
    {
        if (ip == size - 1)
        {
            list->data[ip].next = 0;
        }
        else
        {
            list->data[ip].next = ip + 1;
        }

        list->data[ip].prev = PREV_NO_ELEM;
    }

    list->size = size;
    list->head = 1;
    list->tail = 0;
    list->free = 1;

    list->list_linear = LIST_LINEAR;

    assert_list (list);

    return list;
}

void list_insert_elem (LIST *list, const int value)
{
    assert_list (list);

    CHECK_SIZE ();

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

    CHECK_SIZE ();

    ip = list_get_elem_ip (list, ip);

    if (ip == list->tail)
    {
        list_insert_elem (list, value);
    }
    else
    {
        list->list_linear = LIST_NO_LINEAR;

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

    if (ip != list->tail)
    {
        list->list_linear = LIST_NO_LINEAR;
    }

    assert_list (list);

    return ERR_NO;
}

int list_get_elem_ip (LIST *list, int ip)
{
    assert_list (list);
    my_assert (ip >= 0 && ip < list->size);

    int ip_pos = list->head;

    if (list->list_linear)
    {
        ip_pos = ip;
    }
    else
    {
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
    }

    return ip_pos;
}

void list_realloc (LIST *list)
{
    assert_list (list);

    int size = list->size * HST_UP;

    list->data = (NODE *) realloc (list->data, size * sizeof (NODE));
    my_assert (list->data != NULL);

    list->data[list->free].next = list->size;

    for (int ip = list->size; ip < size; ip++)
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
        list->data[ip].prev  = PREV_NO_ELEM;
    }

    list->size = size;

    assert_list (list);
}

void lineariz_list (LIST *list)
{
    assert_list (list);

    NODE *new_data = (NODE *) calloc (list->size, sizeof (NODE));

    int ip_pos = 0;
    int flag_free = 0;
    list->head = 1;

    for (int ip = 0; ip < list->size; ip++)
    {
        new_data[ip].value = list->data[ip_pos].value;

        if (ip == (list->size - 1) || list->data[ip_pos].next == 0)
        {
            new_data[ip].next = 0;
        }
        else
        {
            new_data[ip].next = ip + 1;
        }

        if (flag_free == 1)
        {
            new_data[ip].prev = PREV_NO_ELEM;
        }
        else
        {
            if (ip != 0)
            {
                new_data[ip].prev  = ip - 1;
            }
        }

        ip_pos = list->data[ip_pos].next;

        if (ip_pos == 0)
        {
            list->tail = ip;
            new_data[0].prev = list->tail;
            list->free = ip + 1;
            flag_free = 1;
        }
    }

    free(list->data);
    list->data = new_data;
    list->list_linear = LIST_LINEAR;

    assert_list (list);
}

void list_move_elem (LIST *list, int ip_1, int ip_2)
{
    assert_list (list);

    int ip_pos_1 = list_get_elem_ip (list, ip_1);
    int ip_pos_2 = list_get_elem_ip (list, ip_2);

    NODE *node_first  = &list->data[ip_pos_1];
    NODE *node_second = &list->data[ip_pos_2];

    int prev_first  = node_first->prev;
    int next_first  = node_first->next;
    int prev_second = node_second->prev;
    int next_second = node_second->next;

    if (next_first == ip_pos_2)
    {
        node_first->prev = ip_pos_2;
        node_first->next = next_second;

        node_second->prev = prev_first;
        node_second->next = ip_pos_1;

        if (ip_pos_1 != list->head)
        {
            list->data[prev_first].next = ip_pos_2;
        }

        if (ip_pos_2 != list->tail)
        {
            list->data[next_second].prev = ip_pos_1;
        }
    }
    else if (next_second == ip_pos_1)
    {
        node_first->prev = prev_second;
        node_first->next = ip_pos_2;

        node_second->prev = ip_pos_1;
        node_second->next = next_first;

        if (ip_pos_2 != list->head)
        {
            list->data[next_second].prev = ip_pos_1;
        }

        if (ip_pos_1 != list->tail)
        {
            list->data[prev_first].next = ip_pos_2;
        }
    }
    else
    {
        list->data[prev_first].next = ip_pos_2;
        list->data[prev_second].next = ip_pos_1;

        node_first->prev = prev_second;
        node_first->next = next_second;

        node_second->prev = prev_first;
        node_second->next = next_first;

        list->data[next_first].prev = ip_pos_2;
        list->data[next_second].prev = ip_pos_1;
    }

    if (ip_pos_1 == list->head)
    {
        list->head = ip_pos_2;
    }
    else if (ip_pos_2 == list->head)
    {
        list->head = ip_pos_1;
    }
    
    if (ip_pos_1 == list->tail)
    {
        list->tail = ip_pos_2;
    }
    else if (ip_pos_2 == list->tail)
    {
        list->tail = ip_pos_1;
    }

    list->list_linear = LIST_NO_LINEAR;

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

    list->size        = LIST_VALUE_VENOM;
    list->head        = LIST_VALUE_VENOM;
    list->tail        = LIST_VALUE_VENOM;
    list->free        = LIST_VALUE_VENOM;
    list->list_linear = LIST_VALUE_VENOM;

    list = NULL;
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

            fprintf (fp_dot, " -> head -> tail -> free[arrowsize = 0.0, weight = %d, color = " BACK_GROUND_COLOR ", fontname = " FONTNAME "];\n", weight);

            for (int ip = 0; ip < list->size; ip++)
            {
                if (list->data[ip].prev != PREV_NO_ELEM && ip != 0)
                {
                    color = LIGHT_GREEN_COLOR;
                }
                else if (ip != 0)
                {
                    color = BLUE_COLOR;
                }
                else
                {
                    color = RED_COLOR;
                }

                fprintf (fp_dot, "\t%d [shape = Mrecord, style = filled, fillcolor = %s, label = \"idx: %d | data: %d | next: %d | prev: %d\"];\n", 
                            ip, color, ip, list->data[ip].value, list->data[ip].next, list->data[ip].prev);
            }
        }

        fprintf (fp_dot, "\thead [shape = Mrecord, style = filled, fillcolor = " LIGHT_GREEN_COLOR ", label = \"head: %d\"];\n", list->head);
        fprintf (fp_dot, "\ttail [shape = Mrecord, style = filled, fillcolor = " LIGHT_GREEN_COLOR ", label = \"tail: %d\"];\n", list->tail);
        fprintf (fp_dot, "\tfree [shape = Mrecord, style = filled, fillcolor = " BLUE_COLOR ", label = \"free: %d\"];\n", list->free);

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
        fprintf (fp_dot, "\tAll[shape = Mrecord, style = filled, fillcolor = " PURPLE_COLOR ", label = \"size = %d\"];}\n", list->size);
    }

    if (fclose (fp_dot) != 0)
    {
        fprintf (stderr, "%s", err_msgs_arr[FILE_CLOSE_ERR]);
    }

    system ("dot -Tsvg dump.dot -o dot.svg");
}

#endif