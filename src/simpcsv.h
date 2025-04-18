#ifndef SIMPCSV_H
#define SIMPCSV_H

#include <stdio.h>

#define CONTENT_INCREMENT (5 * sizeof(char))

typedef struct {
    char* m_data;
    size_t m_capacity;
    size_t m_size;
} _SimpCSV_Da;

_SimpCSV_Da* _simpcsv_da_init(size_t capacity);

_SimpCSV_Da* _simpcsv_da_pushback(_SimpCSV_Da* target, char data);

_SimpCSV_Da* _simpcsv_da_pop(_SimpCSV_Da* target);

void _simpcsv_da_free(_SimpCSV_Da* target);

typedef struct {
    size_t m_col;
    size_t m_row;
    char* m_data;
} _SimpCSV_Cell;

typedef struct {
    FILE* m_file;
    char m_delim;
    char m_escape;
    char* m_contents;
    size_t m_cols;
    size_t m_rows;
    _SimpCSV_Cell* m_cell;
} SimpCSV_Handle;

SimpCSV_Handle* simpcsv_open_file(char* file_name, char delim, char escape);

SimpCSV_Handle* simpcsv_parse_file(SimpCSV_Handle* handle);
//char** simpcsv_get_headers(SimpCSV_Handle* handle);

// char** simpcsv_get_column(SimpCSV_Handle* handle, const char* column_name);
// char** simpcsv_get_row(SimpCSV_Handle* handle, size_t row_number);

// char* simpcsv_get_element(SimpCSV_Handle* handle, const char* column_name, size_t row_number);

void simpcsv_close_file(SimpCSV_Handle* handle);

#endif
