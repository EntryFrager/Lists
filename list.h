#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

static int CODE_ERROR = 0;

#define assert_list(list) {                                             \
    if ((CODE_ERROR = list_verification (list)) != LIST_OK)             \
    {                                                                   \
        list_dump (list, CODE_ERROR, __FILE__, __func__, __LINE__);     \
        abort ();                                                       \
    }                                                                   \
}

enum list_code_error {
    LIST_OK,
    LIST_ERR,
    LIST_ERR_SIZE,
    LIST_ERR_HEAD,
    LIST_ERR_TAIL,
    LIST_ERR_FREE,

    LIST_ERR_DATA,
    LIST_ERR_NEXT,
    LIST_ERR_PREV,

    LIST_POINTER_GARBAGE,

    FILE_OPEN_ERR,
    FILE_CLOSE_ERR,
};

const int LIST_ERROR_CNT = 14;

typedef int ELEMENT;

const ELEMENT LIST_VALUE_VENOM = -100000;

const int PREV_NO_ELEM = -1;

typedef struct LIST {
    int *data = NULL;
    int *next = NULL;
    int *prev = NULL;

    int size = LIST_VALUE_VENOM;
    
    int head = LIST_VALUE_VENOM;
    int tail = LIST_VALUE_VENOM;
    int free = LIST_VALUE_VENOM;
} LIST;

LIST *list_init (size_t size);

void list_insert_elem (LIST *list, int value);

int list_delete_elem (LIST *list, int ip);

void list_realloc (LIST *list);

void list_print (LIST *list);

void list_deinit (LIST *list);

int list_verification (LIST *list);

void list_dump (LIST *list, const int code_error, const char *file_err, const char *func_err, const int line_err);

#endif //LIST_H