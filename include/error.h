/// @file error.h

#ifndef ERROR_H
#define ERROR_H

/**
 * Macro checking the truth of an expression.
 * @param[in] expr Expression
*/

#define my_assert(expr) if (!(expr)) {  \
        fprintf(stderr, "\x1b[31m%s %s:%d: My assertion failed: \"" #expr "\"\x1b[0m\n", __FILE__, __func__, __LINE__); \
        abort (); \
    }

const int ERROR_CNT = 11;                       ///< Number of possible errors provided by the author.

enum code_error {                               ///< Error codes.
    ERR_NO,                                     ///< No error.
    ERR_FOPEN,                                  ///< Error opening file.
    ERR_FCLOSE,                                 ///< Error closing file.
    ERR_FREAD,                                  ///< Error when reading text from file.
    ERR_DELETE,
    ERR_INSERT,
};

const char* my_strerr (unsigned code_error);    ///< Error return function.

#endif // ERROR_H
