// simpcsv.h
// Copyright (C) David Filiks

#ifndef SIMPCSV_H
#define SIMPCSV_H

#include <stdio.h>

typedef struct
{
    char m_quote; // Stores the quote character used in the CSV file.
    char m_delim; // Stores the delimiter character used in the CSV file.
    char m_escape; // Stores the escape character used in the CSV file.
    char* m_source; // Stores the CSV file contents as a string.
    size_t m_source_size; // Stores the length of the CSV file in characters.
    size_t m_number_of_rows; // Stores the number of rows in the CSV file. Uninitialized until simpcsv_count_rows_and_cols() is called.
    size_t m_number_of_cols; // Stores the number of columns in the CSV file. Uninitialized until simpcsv_count_rows_and_cols() is called.
    char* _m_buffer; // Buffer that is used for parsing. Shouldn't be touched by user.
} SimpCSVHandle;

// @brief Allocates more memory to the buffer if needed. Shouldn't be touched by user.
void _simpcsv_handle_buffer(SimpCSVHandle* handle, size_t index, size_t* size);

// @brief Opens the CSV file specified using memory mapping.
SimpCSVHandle* simpcsv_open_file(char* file_name, char quote, char delim, char escape);

// @brief Gets cell at the specified row and column. 1st cell is (row: 0, col: 0).
char* simpcsv_get_cell(SimpCSVHandle* handle, size_t row, size_t col);

// @brief Counts the number of rows and columns in the CSV file. Stores them in handle->m_number_of_rows and handle->m_number_of_cols.
SimpCSVHandle* simpcsv_count_rows_and_cols(SimpCSVHandle* handle);

// @brief Unmaps the CSV file and deallocates all dynamically allocated memory.
void simpcsv_close_file(SimpCSVHandle* handle);

#endif
