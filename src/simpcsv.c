#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "simpcsv.h"

_SimpCSV_Da* _simpcsv_da_init(size_t capacity)
{
    _SimpCSV_Da* da = malloc(sizeof(_SimpCSV_Da));
    da->m_data = malloc(capacity * sizeof(char*));
    da->m_capacity = capacity;
    da->m_size = 0;

    return da;
}

_SimpCSV_Da* _simpcsv_da_resize(_SimpCSV_Da* target, size_t new_capacity)
{
    void* copy = malloc(new_capacity * sizeof(char*));
    memcpy(target->m_data, copy, sizeof(copy));
    free(copy);
    return target;
}

_SimpCSV_Da* _simpcsv_da_pushback(_SimpCSV_Da* target, char data)
{
    if (target->m_capacity == target->m_size)
    {
        return NULL; // RESIZE
    }
    target->m_data[target->m_size] = data;
    target->m_size++;

    return target;
}

_SimpCSV_Da* _simpcsv_da_pop(_SimpCSV_Da* target)
{
    assert(target->m_size > 0);

    target->m_size--;

    target = _simpcsv_da_resize(target, target->m_size);

    // RESIZE to take less space

    return target;
}

void _simpcsv_da_free(_SimpCSV_Da* target)
{
    free(target->m_data);
    free(target);
}

SimpCSV_Handle* simpcsv_open_file(char* file_name, char delim, char escape)
{
    SimpCSV_Handle* handle = malloc(sizeof(SimpCSV_Handle));
    handle->m_delim = delim;
    handle->m_escape = escape;

    handle->m_file = fopen(file_name, "r");
    int c;
    size_t i = 0, size = CONTENT_INCREMENT;
    handle->m_contents = malloc(size);

    while ((c = fgetc(handle->m_file)) != EOF)
    {
        if (i == size - 1)
        {
            size += CONTENT_INCREMENT;
            handle->m_contents = realloc(handle->m_contents, size);
        }

        handle->m_contents[i++] = c;
    }

    handle->m_contents[i] = '\0';

    return handle;
}

SimpCSV_Handle* simpcsv_parse_file(SimpCSV_Handle* handle)
{
    handle->m_cell = malloc(200 * sizeof(_SimpCSV_Cell));
//    _SimpCSV_Cell* cell = malloc(sizeof(_SimpCSV_Cell));
    _SimpCSV_Da* buffer = _simpcsv_da_init(5000);

    for (size_t i = 0; i < strlen(handle->m_contents); i++)
    {
        while (handle->m_contents[i] != handle->m_delim && handle->m_contents[i] != '\0')
        {
            buffer = _simpcsv_da_pushback(buffer, handle->m_contents[i++]);
        }

        handle->m_cell[i].m_data = buffer->m_data;
        buffer = _simpcsv_da_resize(buffer, 0);
    }

    _simpcsv_da_free(buffer);

    return handle;
}

// parse the file, assign a number to each column, assign a column number to each cell. this function will just
// find all the cells in the specific column and return an array of them.
// char** simpcsv_get_column(SimpCSV_Handle* handle, const char* column_name);

// char** simpcsv_get_row(SimpCSV_Handle* handle, size_t row_number);

// char* simpcsv_get_element(SimpCSV_Handle* handle, const char* column_name, size_t row_number);

void simpcsv_close_file(SimpCSV_Handle* handle)
{
    fclose(handle->m_file);
    free(handle->m_contents);
    free(handle);
}
