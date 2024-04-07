#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../error/error.h"
#include "../utilities/utils.h"

#ifdef DEBUG
    #define assert_list {                       \
        list_verification(list, code_error);    \
        if (*code_error != ERR_NO)              \
        {                                       \
            CALL_DUMP(list, code_error);        \
        }                                       \
    }

    #define CALL_DUMP(list, code_error)                                         \
    {                                                                           \
        list_dump_text(list, code_error, __FILE__, __func__, __LINE__);         \
        list_dump_graph_viz(list, code_error, __FILE__, __func__, __LINE__);    \
    }
#endif

typedef int ELEMENT;

const int LIST_VALUE_VENOM = 0xABCDEF;

const int FICTIV_ELEM_ID = 0;

const int POISON_VALUE = 0;
const int PREV_NO_ELEM = -1;

const bool LIST_LINEAR    = true;
const bool LIST_NO_LINEAR = false;
const bool LIST_INIT      = true;
const bool LIST_NO_INIT   = false;

const int UP   = 1;
const int DOWN = 2;

static const int COEF_UP = 2;

typedef struct {
    ELEMENT value;
    int next = LIST_VALUE_VENOM;
    int prev = LIST_VALUE_VENOM;
} NODE;

typedef struct {
    NODE *data = {};

    int size = LIST_VALUE_VENOM;

    int head = LIST_VALUE_VENOM;
    int tail = LIST_VALUE_VENOM;
    int free = LIST_VALUE_VENOM;

    bool list_init = false;
    bool list_linear = false;
} List;

void list_init (List *list, const int size, int *code_error);

void list_insert_elem (List *list, const ELEMENT value, int *code_error);

void list_insert_elem_after_log (List *list, const ELEMENT value, const int ip, int *code_error);

void list_insert_elem_after_ph (List *list, const ELEMENT value, int ip, int *code_error);

void list_delete_elem (List *list, const int ip, int *code_error);

void list_check_realloc (List *list, int *code_error);

void list_realloc (List *list, const int mode, int *code_error);

void lineariz_list (List *list, int *code_error);

void list_swap_elem (List *list, const int ip_1, const int ip_2, int *code_error);

void list_print (const List *list, int *code_error);

void list_deinit (List *list, int *code_error);

#ifdef DEBUG
    void list_verification (const List *list, int *code_error);

    void list_dump_text (const List *list, int *code_error,
                         const char *file_err, const char *func_err,
                         const int line_err);

    void list_dump_graph_viz (const List *list, int *code_error,
                              const char *file_err, const char *func_err,
                              const int line_err);

    void list_dump_html (int *code_error);
#endif

#endif //LIST_H
