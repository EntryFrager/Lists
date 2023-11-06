#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

typedef int ELEMENT;

const int VALUE_DEFAULT = 0;

const ELEMENT VALUE_VENOM = 0xDEDEDEDE;

typedef struct LIST {
    int data = 0;

    LIST *next = NULL;
    LIST *prev = NULL;
} LIST;

LIST *list_init (int value);

LIST *list_insert_elem (LIST *list, int value);

LIST *list_delete_elem (LIST *list);

void list_print (LIST *list);

LIST *list_move_elem (LIST *list_first, LIST *list_second, LIST *head);

#endif //LIST_H