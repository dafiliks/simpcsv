#ifndef SIMPCSV_H
#define SIMPCSV_H

#include <stdio.h>

#define _SIMPCSV_CHAR_INCREMENT (5 * sizeof(char))
#define _SIMPCSV_CELL_INCREMENT (5 * sizeof(_SimpCSV_Cell))

#define _SIMPCSV_CELL_CHECK_IF_RESIZE_NEEDED(buffer, index, size, inc)   \
    if (index == size)                                                   \
    {                                                                    \
        size += inc;                                                     \
        buffer = realloc(buffer, size);                                  \
    }                                                                    \

#define _SIMPCSV_CHAR_CHECK_IF_RESIZE_NEEDED(buffer, index, size, inc)   \
    size_t k = size; /* dumb c string stuff */                           \
    if (index == k)                                                      \
    {                                                                    \
        bs += inc;                                                       \
        buffer = realloc(buffer, bs);                                    \
    }                                                                    \

typedef struct {
    size_t m_col;
    size_t m_row;
    char* m_data;
} _SimpCSV_Cell;

typedef struct {
    FILE* m_file;
    char m_quote;
    char m_delim;
    char m_escape;
    char* m_contents;
    size_t m_cols;
    size_t m_rows;
    _SimpCSV_Cell* m_cell;
    size_t m_number_of_cells;
    size_t m_number_of_cols;
    size_t m_number_of_rows;
} SimpCSV_Handle;

SimpCSV_Handle* simpcsv_open_file(char* file_name, char quote, char delim, char escape);

SimpCSV_Handle* simpcsv_parse_file(SimpCSV_Handle* handle);

size_t simpcsv_get_number_of_columns(SimpCSV_Handle* handle);

size_t simpcsv_get_number_of_rows(SimpCSV_Handle* handle);

/* DRAFTS */

//char** simpcsv_get_headers(SimpCSV_Handle* handle);
// char** simpcsv_get_column(SimpCSV_Handle* handle, const char* column_name);
// char** simpcsv_get_row(SimpCSV_Handle* handle, size_t row_number);
// char* simpcsv_get_element(SimpCSV_Handle* handle, const char* column_name, size_t row_number);

/* DRAFTS */

void simpcsv_close_file(SimpCSV_Handle* handle);

#endif
