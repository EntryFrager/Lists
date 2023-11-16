/// @file error.cpp

#include "error.h"

/**
 * Types of errors.
*/

static const char* err_msgs_arr[] = {
    "ERROR NO.\n",
    "ERROR: an error occurred while opening the file.\n",
    "ERROR: an error occurred while closing the file.\n",
    "ERROR: an error occurred while reading text from the file.\n",
    "ERROR: error when deleting list item.\n",
    "ERROR: error when inserting element into list.\n"
    "ERROR: error when resizing list.\n",
};

/**
 * Error return function.
 * @param[in] code_error
*/

const char* my_strerr (unsigned code_error)
{
    if (code_error < ERROR_CNT)
    {
        return err_msgs_arr[code_error];
    }
    else
    {
        return "Unknown error.\n";
    }
}
