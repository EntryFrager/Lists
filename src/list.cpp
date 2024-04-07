#include "../include/list.h"

#define WHITE_COLOR "\"#ffffff\""
#define BLUE_COLOR "\"#00BFFF\""
#define YELLOW_COLOR "\"#FFFF00\""
#define PURPLE_COLOR "\"#8B00FF\""
#define RED_COLOR "\"#ff0000\""
#define LIGHT_GREEN_COLOR "\"#ccff99\""
#define BACK_GROUND_COLOR "\"#696969\""
#define WEIGHT "\"10000\""

#define FONTNAME "\"Times-New-Roman\""

static const char *FP_ERR_NAME          = "./debug/file_err.txt";
static const char *FP_DOT_NAME          = "./debug/dump.dot";
static const char *FP_HTML_DOT_NAME     = "./debug/dot.html";
static const char *FP_IMAGE             = "./debug/dot.svg";

static const char *CMD_COMPILE_DOT_FILE = "dot -Tsvg ./debug/dump.dot -o ./debug/dot.svg";

static int list_get_elem_index (const List *list, const int index, int *code_error);
static void list_filling (List *list, const int index, int *code_error);

void list_init (List *list, const int size, int *code_error)
{
    my_assert(list != NULL, ERR_PTR);
    my_assert(size > 0, ERR_LIST_SIZE);

    list->data = (NODE *) calloc(size, sizeof (NODE));
    my_assert(list->data != NULL, ERR_MEM);

    list->size = size;

    list->head = 0;
    list->tail = 0;
    list->free = 1;

    list_filling(list, FICTIV_ELEM_ID, code_error);

    list->list_linear = LIST_LINEAR;
    list->list_init   = LIST_INIT;

    assert_list;
}

void list_filling (List *list, const int index, int *code_error)
{
    my_assert(list != NULL, ERR_PTR);

    for (int ip = index; ip < list->size; ip++)
    {
        if (ip == list->size - 1)
        {
            list->data[ip].next = 0;
        }
        else
        {
            list->data[ip].next = ip + 1;
        }

        list->data[ip].value = POISON_VALUE;
        list->data[ip].prev  = PREV_NO_ELEM;
    }
}

void list_insert_elem (List *list, const ELEMENT value, int *code_error)
{
    assert_list;

    list_insert_elem_after_ph(list, value, list->tail, code_error);

    assert_list;
}

void list_insert_elem_after_ph (List *list, const ELEMENT value, const int ip, int *code_error)
{
    my_assert(ip >= 0 && ip <= list->size, ERR_LIST_INSERT);
    assert_list;

    list_check_realloc(list, code_error);

    int free_next = list->data[list->free].next;
    list->data[list->free].value = value;

    if (ip == list->tail)
    {
        list->data[list->free].prev = list->tail;
        list->data[list->free].next = FICTIV_ELEM_ID;

        list->data[FICTIV_ELEM_ID].prev = list->free;

        list->data[list->tail].next = list->free;

        list->tail = list->free;
    }
    else
    {
        list->list_linear = LIST_NO_LINEAR;

        if (ip == 0)
        {
            list->head = list->free;
        }

        list->data[list->free].next = list->data[ip].next;
        list->data[list->free].prev = ip;

        list->data[list->data[ip].next].prev = list->free;
        list->data[ip].next = list->free;
    }

    if (list->head == 0)
    {
        list->head = list->free;
    }

    list->free = free_next;

    assert_list;
}

void list_insert_elem_after_log (List *list, const ELEMENT value, int ip, int *code_error)
{
    my_assert(ip >= 0 && ip <= list->size, ERR_LIST_INSERT);
    assert_list;

    ip = list_get_elem_index(list, ip, code_error);
    list_insert_elem_after_ph(list, value, ip, code_error);

    assert_list;
}

void list_delete_elem (List *list, int ip, int *code_error)
{
    my_assert(ip <= list->size, ERR_LIST_DELETE);
    my_assert(list->data[ip].prev != PREV_NO_ELEM, ERR_LIST_DELETE);

    assert_list;

    ip = list_get_elem_index(list, ip, code_error);

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

    assert_list;
}

int list_get_elem_index (const List *list, const int index, int *code_error)
{
    assert_list;

    int ip_pos = list->head;

    if (list->list_linear)
    {
        ip_pos = index;
    }
    else
    {
        if (index == 0)
        {
            ip_pos = index;
        }
        else
        {
            for (int counter = 0; counter < index - 1; counter++)
            {
                ip_pos = list->data[ip_pos].next;
            }
        }
    }

    return ip_pos;
}

void list_check_realloc (List *list, int *code_error)
{
    assert_list;

    if (list->free == (list->size - 1))
    {
        list_realloc(list, UP, code_error);
    }

    assert_list;
}

void list_realloc (List *list, const int mode, int *code_error)
{
    my_assert(mode == DOWN || mode == UP, ERR_LIST_REALLOC);
    assert_list;

    int size = 0;

    if (mode == UP)
    {
        size = list->size * COEF_UP;
    }
    else if (mode == DOWN)
    {
        lineariz_list(list, code_error);

        size = list->size - (list->size - list->free);
    }

    list->data = (NODE *) realloc(list->data, size * sizeof (NODE));
    my_assert(list->data != NULL, ERR_MEM);

    int temp_size = list->size;
    list->size = size;

    if (mode == UP)
    {
        list->data[list->free].next = temp_size;

        list_filling(list, temp_size, code_error);
    }
    else
    {
        list->free = 0;
    }

    assert_list;
}

void lineariz_list (List *list, int *code_error)
{
    assert_list;

    NODE *new_data = (NODE *) calloc(list->size, sizeof (NODE));
    my_assert(new_data != NULL, ERR_MEM);

    int ip_log = 0;
    int ip_ph = list->head;
    list->head = 1;

    do
    {
        new_data[++ip_log].value = list->data[ip_ph].value;

        if (list->data[ip_ph].next == 0)
        {
            new_data[ip_log].next = 0;
        }
        else
        {
            new_data[ip_log].next = ip_log + 1;
        }

        new_data[ip_log].prev = ip_log - 1;

        ip_ph = list->data[ip_ph].next;
    } while (ip_ph != 0);

    list->tail = ip_log;
    list->free = ip_log + 1;

    new_data[FICTIV_ELEM_ID].next = 1;
    new_data[FICTIV_ELEM_ID].prev = list->tail;

    free(list->data);
    list->data = new_data;

    list_filling(list, list->free, code_error);

    list->list_linear = LIST_LINEAR;

    assert_list;
}

void list_swap_elem (List *list, const int ip_1, const int ip_2, int *code_error)
{
    assert_list;

    int ip_pos_1 = list_get_elem_index(list, ip_1, code_error);
    int ip_pos_2 = list_get_elem_index(list, ip_2, code_error);

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

    assert_list;
}

void list_print (const List *list, int *code_error)
{
    assert_list;

    int ip = list->head;

    while (ip != 0)
    {
        printf("%d\n", list->data[ip].value);

        ip = list->data[ip].next;
    }
}

void list_deinit (List *list, int *code_error)
{
    assert_list;

    free(list->data);

    list->data = NULL;

    list->size        = LIST_VALUE_VENOM;
    list->head        = LIST_VALUE_VENOM;
    list->tail        = LIST_VALUE_VENOM;
    list->free        = LIST_VALUE_VENOM;
    list->list_linear = LIST_NO_LINEAR;
    list->list_init   = LIST_NO_INIT;

    list = NULL;
}

#ifdef DEBUG

void list_verification (const List *list, int *code_error)
{
    if (list == NULL)
    {
        *code_error |= ERR_LIST;
    }

    if (list->list_init != LIST_INIT)
    {
        *code_error |= LIST_POINTER_GARBAGE;
    }

    if (list->size <= 0)
    {
        *code_error |= ERR_LIST_SIZE;
    }

    if (list->data == NULL)
    {
        *code_error |= ERR_LIST_DATA;
    }

    if (list->head < 0 || list->head >= list->size)
    {
        *code_error |= ERR_LIST_HEAD;
    }

    if (list->head < 0 || list->tail >= list->size)
    {
        *code_error |= ERR_LIST_TAIL;
    }

    if (list->head < 0 || list->free >= list->size)
    {
        *code_error |= ERR_LIST_FREE;
    }

    int ip = list->head;
    int counter = 0;

    if (ip != 0)
    {
        do {
            if (list->data[list->data[ip].next].prev != ip)
            {
                *code_error |= ERR_LIST_PTR_DATA;
                break;
            }

            if (ip == list->data[ip].next)
            {
                *code_error |= ERR_LIST_DATA_LOOP;
                break;
            }

            ip = list->data[ip].next;
            counter++;
        } while (ip != 0);
    }

    if (list->free > 0)
    {
        ip = list->free;

        do {
            if (list->data[ip].prev != PREV_NO_ELEM)
            {
                *code_error |= ERR_LIST_PTR_FREE;
                break;
            }

            if (ip == list->data[ip].next)
            {
                *code_error |= ERR_LIST_FREE_LOOP;
                break;
            }


            ip = list->data[ip].next;
            counter++;
        } while (ip != 0);
    }

    if (counter != (list->size - 1))
    {
        *code_error |= ERR_LIST_CNT_NODE;
    }
}

#define DUMP_LOG(str) fprintf(fp_err, str);
#define DUMP_LOG_PARAM(str, ...) fprintf(fp_err, str, __VA_ARGS__);

void list_dump_text (const List *list, int *code_error, const char *file_err, const char *func_err, const int line_err)
{
    FOPEN_(fp_err, FP_ERR_NAME, "a");

    if (fp_err == NULL)
    {
        my_strerr(*code_error, stderr);
    }
    else if (list != NULL)
    {
        my_strerr(*code_error, fp_err);

        DUMP_LOG_PARAM("list[%p] \"list\" called from %s(%d) %s\n", list, file_err, line_err, func_err);

        DUMP_LOG("{\n");

        DUMP_LOG_PARAM("\tsize = %d\n", list->size);
        DUMP_LOG_PARAM("\thead = %d\n", list->head);
        DUMP_LOG_PARAM("\ttail = %d\n", list->tail);
        DUMP_LOG_PARAM("\tfree = %d\n", list->free);

        if (list->data != NULL)
        {
            DUMP_LOG_PARAM("\tdata[%p]\n", list->data);
            DUMP_LOG("\t{\n");

            if (!(*code_error & ERR_LIST_SIZE))
            {
                for (int ip = 0; ip < list->size; ip++)
                {
                    DUMP_LOG_PARAM("\t\tdata[%d]->value = %d\n", ip, list->data[ip].value);
                    DUMP_LOG_PARAM("\t\tdata[%d]->next  = %d\n", ip, list->data[ip].next);
                    DUMP_LOG_PARAM("\t\tdata[%d]->prev  = %d\n\n", ip, list->data[ip].prev);
                }
            }

            DUMP_LOG("\t}\n");
        }
        else
        {
            DUMP_LOG("\tdata[NULL]\n");
        }

        DUMP_LOG("}\n\n-----------------------------------------------------------\n");
    }
    else
    {
        my_strerr(*code_error, fp_err);

        DUMP_LOG_PARAM("list[NULL] \"list\" called from %s(%d) %s\n", file_err, line_err, func_err);
    }

    FCLOSE_(fp_err);
}

#undef DUMP_LOG
#undef DUMP_LOG_PARAM

#define DUMP_DOT(str) fprintf(fp_dot, str);
#define DUMP_DOT_PARAM(str, ...) fprintf(fp_dot, str, __VA_ARGS__);

void list_dump_graph_viz (const List *list, int *code_error, const char *file_err, const char *func_err, const int line_err)
{
    FOPEN_(fp_dot, FP_DOT_NAME, "w+");

    const char *color = WHITE_COLOR;

    if (fp_dot == NULL)
    {
        my_strerr(*code_error, stderr);
    }
    else if (list != NULL)
    {
        DUMP_DOT("digraph List {\n");
        DUMP_DOT("\trankdir = LR;\n");
        DUMP_DOT("\tnode [shape = record];\n");
        DUMP_DOT("\tbgcolor = " BACK_GROUND_COLOR ";\n");

        if (!(*code_error & ERR_LIST_SIZE))
        {
            DUMP_DOT("\t0 -> 0");

            for (int ip = 1; ip < list->size; ip++)
            {
                DUMP_DOT_PARAM(" -> %d", ip);
            }

            DUMP_DOT(" -> head -> tail -> free[arrowsize = 0.0, weight = " WEIGHT ", color = " BACK_GROUND_COLOR ", fontname = " FONTNAME "];\n");

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

                DUMP_DOT_PARAM("\t%d [shape = Mrecord, style = filled, fillcolor = %s, label = \"idx: %d | data: %d | next: %d | prev: %d\"];\n",
                               ip, color, ip, list->data[ip].value, list->data[ip].next, list->data[ip].prev);
            }
        }

        DUMP_DOT_PARAM("\thead [shape = Mrecord, style = filled, fillcolor = " LIGHT_GREEN_COLOR ", label = \"head: %d\"];\n", list->head);
        DUMP_DOT_PARAM("\ttail [shape = Mrecord, style = filled, fillcolor = " LIGHT_GREEN_COLOR ", label = \"tail: %d\"];\n", list->tail);
        DUMP_DOT_PARAM("\tfree [shape = Mrecord, style = filled, fillcolor = " BLUE_COLOR        ", label = \"free: %d\"];\n", list->free);

        int ip = 0;

        do
        {
            DUMP_DOT_PARAM("\t%d -> %d\n", ip, list->data[ip].next);

            ip = list->data[ip].next;
        } while (ip != 0);

        if (!(*code_error & ERR_LIST_FREE))
        {
            ip = list->free;

            while (ip != 0)
            {
                DUMP_DOT_PARAM("\t%d -> %d\n", ip, list->data[ip].next);

                ip = list->data[ip].next;
            }
        }

        DUMP_DOT_PARAM("\thead -> %d\n", list->head);
        DUMP_DOT_PARAM("\ttail -> %d\n", list->tail);
        DUMP_DOT_PARAM("\tfree -> %d\n", list->free);

        DUMP_DOT_PARAM("\tlabel = \"list_dump from function %s, List/%s:%d\";\n", func_err, file_err, line_err);
        DUMP_DOT_PARAM("\tAll[shape = Mrecord, style = filled, fillcolor = " PURPLE_COLOR ", label = \"size = %d\"];}\n", list->size);
    }

    FCLOSE_(fp_dot);

    system(CMD_COMPILE_DOT_FILE);

    list_dump_html(code_error);
}

#undef DUMP_DOT
#undef DUMP_DOT_PARAM

void list_dump_html (int *code_error)
{
    FOPEN_(fp_dot, FP_IMAGE, "r");
    FOPEN_(fp_html_dot, FP_HTML_DOT_NAME, "a");

    size_t size_dot = get_file_size(fp_dot, code_error);

    char *data_dot = (char *) calloc(size_dot, sizeof(char));
    my_assert(data_dot != NULL, ERR_MEM);

    fread(data_dot, sizeof (char), size_dot, fp_dot);

    fprintf(fp_html_dot, "%s\n", data_dot);

    FCLOSE_(fp_html_dot);
    FCLOSE_(fp_dot);
}

#endif
