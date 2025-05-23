// simpcsv.c
// Copyright (C) David Filiks

#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <assert.h>
#include "simpcsv.h"

// @param[in] *file_name The name of the CSV file.
// @param[in] quote The quote character for the CSV file.
// @param[in] delim The delimiter character for the CSV file.
// @param[in] escape The escape character for the CSV file.
// @returns A SimpCSVHandle to the memory mapped CSV file.
SimpCSVHandle* simpcsv_open_file(char* file_name, char quote, char delim, char escape)
{
    SimpCSVHandle* handle = malloc(sizeof(SimpCSVHandle));

    handle->m_quote = quote;
    handle->m_delim = delim;
    handle->m_escape = escape;

    int fd = open(file_name, O_RDONLY);

    struct stat fs;

    fstat(fd, &fs);

    handle->m_source_size = fs.st_size;

    handle->m_source = mmap(NULL, handle->m_source_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

    handle->m_source[handle->m_source_size] = '\0';

    handle->_m_buffer = NULL;

    close(fd);

    return handle;
}

// @param[in] handle Handle to the open CSV file.
// @param[in] row The desired cell's row number.
// @param[in] col The desired cell's column number.
// @returns A string of the contents in the specified cell.
char* simpcsv_get_cell(SimpCSVHandle* handle, size_t row, size_t col)
{
    char* source_ptr = handle->m_source;

    char target[3];
    target[0] = handle->m_delim;
    target[1] = handle->m_quote;
    target[2] = '\0';

    for (size_t i = 0; i < row; i++)
    {
        source_ptr = strchr(source_ptr, handle->m_escape) + 1;
    }

    for (size_t i = 0; i < col; i++)
    {
        source_ptr = strpbrk(source_ptr, target);

        if (*source_ptr == handle->m_quote)
        {
            source_ptr++;
            source_ptr = strchr(source_ptr, handle->m_quote) + 1;

            while (*source_ptr != handle->m_delim &&
                   *source_ptr != handle->m_escape &&
                   *source_ptr != '\0')
            {
                source_ptr = strchr(source_ptr, handle->m_quote) + 1;
            }
        }

        source_ptr++;
    }

    size_t bs = 100, bi = 0, i = 0;

    if (handle->_m_buffer)
    {
        free(handle->_m_buffer);
    }

    handle->_m_buffer = calloc(bs, 1);

    while (source_ptr[i] != handle->m_delim &&
           source_ptr[i] != handle->m_escape)
    {
        _simpcsv_handle_buffer(handle, i + 1, &bs);

        if (source_ptr[i] == handle->m_quote)
        {
            i++;
            while (source_ptr[i] != handle->m_quote ||
                  (source_ptr[i + 1] != handle->m_delim &&
                   source_ptr[i + 1] != handle->m_escape &&
                   source_ptr[i + 1] != '\0'))
            {
                _simpcsv_handle_buffer(handle, i + 1, &bs);

                handle->_m_buffer[bi++] = source_ptr[i++];
            }

            break;
        }

        handle->_m_buffer[bi++] = source_ptr[i++];
    }

    handle->_m_buffer[bi] = '\0';

    return handle->_m_buffer;
}

// @param[in] handle Handle to the open CSV file.
// @returns Returns the handle with updated m_number_of_rows and m_number_of_cols values.
SimpCSVHandle* simpcsv_count_rows_and_cols(SimpCSVHandle* handle)
{
    char* source_ptr = handle->m_source;
    size_t delim_count = 0, escape_count = 0;

    bool are_all_cols_counted = false;

    char target[4];
    target[0] = handle->m_delim;
    target[1] = handle->m_quote;
    target[2] = handle->m_escape;
    target[3] = '\0';

    while (*source_ptr != '\0')
    {
        source_ptr = strpbrk(source_ptr, target);

        if (*source_ptr == handle->m_quote)
        {
            source_ptr++;
            source_ptr = strchr(source_ptr, handle->m_quote);
        }

        else if (*source_ptr == handle->m_delim)
        {
            if (are_all_cols_counted == false) delim_count++;
        }

        else if (*source_ptr == handle->m_escape)
        {
            escape_count++;
            are_all_cols_counted = true;
        }

        source_ptr++;
    }

    handle->m_number_of_rows = escape_count;
    handle->m_number_of_cols = ++delim_count; // ++ because last cell in row doesn't get counted

    return handle;
}

// @param[in] handle Handle to the open CSV file.
void simpcsv_close_file(SimpCSVHandle* handle)
{
    munmap(handle->m_source, handle->m_source_size);
    free(handle->_m_buffer);
    free(handle);
}

// @param[in] handle Handle to the open CSV file.
// @param[in] index The current index of the loop.
// @param[in] *size The current size of the buffer.
void _simpcsv_handle_buffer(SimpCSVHandle* handle, size_t index, size_t* size)
{
    if (index == *size)
    {
        *size += 100;
        handle->_m_buffer = realloc(handle->_m_buffer, *size);
    }
}
