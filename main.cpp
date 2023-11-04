#include "list.h"

int main ()
{
    LIST list = {};

    list_ctor (&list);

    list_dtor (&list);

    return 0;
}