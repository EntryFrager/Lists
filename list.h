#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

#include "..\include\error.h"

typedef int ELEMENT;

const int VALUE_DEFAULT = 0;

const ELEMENT VALUE_VENOM = 0xDEDEDEDE;

typedef struct {
    ELEMENT *buf = NULL;

    size_t *next = NULL;
    size_t *prev = NULL;

    size_t size = VALUE_DEFAULT;

    size_t tale = VALUE_DEFAULT;
    size_t head = VALUE_DEFAULT;
    size_t free = VALUE_DEFAULT;
} LIST;

void list_ctor (LIST *list);

void list_dtor (LIST *list);

#endif //LIST_H