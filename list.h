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
    #define CHECK_ERROR(code_error) if (code_error != ERR_NO) fprintf (stderr, "\x1b[31m%s\x1b[0m", my_strerr (code_error));
#else
    #define CHECK_ERROR(...)
#endif

// TD: CALL_DUMP(dump_func);

#ifdef DEBUG
    #define assert_list(list) {                                                     \
        if ((CODE_ERROR = list_verification (list)) != LIST_OK)                     \
        {                                                                           \
            list_dump_text (list, CODE_ERROR, __FILE__, __func__, __LINE__);        \
            list_dump_graph_viz (list, CODE_ERROR, __FILE__, __func__, __LINE__);   \
            abort ();                                                               \
        }                                                                           \
    }
#endif

// TD: abort, too hard

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

const int POISON_VALUE   = 0;
const int PREV_NO_ELEM    = -1;
const bool LIST_LINEAR    = 1;
const bool LIST_NO_LINEAR = 0;

//TODO инкапсуляция

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
    
    bool list_linear = LIST_VALUE_VENOM;
} LIST;

LIST *list_init (LIST *list, const size_t size);

void list_insert_elem (LIST *list, const int value);

int list_insert_elem_after_log (LIST *list, const int value, int ip);

int list_insert_elem_after_ph (LIST *list, const int value, int ip);

int list_delete_elem (LIST *list, const int ip);

void list_realloc (LIST *list, const int mode);

void lineariz_list (LIST *list);

void list_swap_elem (LIST *list, int ip_1, int ip_2);

void list_print (const LIST *list);

void list_deinit (LIST *list);

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