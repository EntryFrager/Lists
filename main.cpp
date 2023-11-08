#include "list.h"

int main ()
{
    LIST *list = list_init (5);

    for (size_t ip = 1; ip < 12; ip++)
    {
        list_insert_elem (list, ip);
    }

    list_print (list);

    return 0;
}