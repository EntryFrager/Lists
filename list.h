#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// g++ -I
#include "include\error.h"
#include "include\utils.h"

static int CODE_ERROR = 0;

#define DEBUG

#ifdef DEBUG
    #define CHECK_ERROR_MAIN(code_error) if (code_error != ERR_NO) fprintf (stderr, "\x1b[31m%s\x1b[0m", my_strerr (code_error));
    #define CHECK_ERROR_FUNC(code_error) if (code_error != ERR_NO) return code_error;
#else
    #define CHECK_ERROR_MAIN(...)
    #define CHECK_ERROR_FUNC(...)
#endif

#define CALL_DUMP(list, code_error)                                             \
{                                                                               \
    list_dump_text (list, code_error, __FILE__, __func__, __LINE__);            \
    list_dump_graph_viz (list, code_error, __FILE__, __func__, __LINE__);       \
}

#ifdef DEBUG
    #define assert_list(list) {                                                     \
        if ((CODE_ERROR = list_verification (list)) != LIST_OK)                     \
        {                                                                           \
            CALL_DUMP (list, CODE_ERROR);                                           \
            return CODE_ERROR;                                                      \
        }                                                                           \
    }
#endif

enum list_code_error {
    LIST_OK,
    LIST_ERR,
    LIST_ERR_SIZE,
    LIST_ERR_HEAD,
    LIST_ERR_TAIL,
    LIST_ERR_FREE,

    LIST_ERR_DATA,
    LIST_ERR_PTR_DATA,
    LIST_ERR_PTR_FREE,
    LIST_ERR_CNT_NODE,
    LIST_ERR_DATA_LOOP,
    LIST_ERR_FREE_LOOP,

    LIST_POINTER_GARBAGE,

    FILE_OPEN_ERR,
    FILE_CLOSE_ERR,

    LIST_ERROR_CNT
};

typedef int ELEMENT;

const ELEMENT LIST_VALUE_VENOM = -100000;

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
    int value = LIST_VALUE_VENOM;
    int next = LIST_VALUE_VENOM;
    int prev = LIST_VALUE_VENOM;
} NODE;

typedef struct LIST {
    NODE *data = {};

    int size = LIST_VALUE_VENOM;
    
    int head = LIST_VALUE_VENOM;
    int tail = LIST_VALUE_VENOM;
    int free = LIST_VALUE_VENOM;

    bool list_init = false;
    bool list_linear = false;
} LIST;

int list_init (LIST *list, const size_t size);

int list_insert_elem (LIST *list, const int value);

int list_insert_elem_after_log (LIST *list, const int value, int ip);

int list_insert_elem_after_ph (LIST *list, const int value, int ip);

int list_delete_elem (LIST *list, const int ip);

int list_check_realloc (LIST *list);

int list_realloc (LIST *list, const int mode);

int lineariz_list (LIST *list);

int list_swap_elem (LIST *list, int ip_1, int ip_2);

int list_print (const LIST *list);

int list_deinit (LIST *list);

#ifdef DEBUG
    int list_verification (const LIST *list);

    void list_dump_text (const LIST *list, const int code_error, 
                         const char *file_err, const char *func_err, 
                         const int line_err);

    void list_dump_graph_viz (const LIST *list, const int code_error, 
                              const char *file_err, const char *func_err, 
                              const int line_err);

    void list_dump_html ();
#endif

#endif //LIST_H