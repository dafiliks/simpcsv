#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "simpcsv.h"

SimpCSV_Handle* simpcsv_open_file(char* file_name, char quote, char delim, char escape)
{
    SimpCSV_Handle* handle = malloc(sizeof(SimpCSV_Handle));

    handle->m_quote = quote;
    handle->m_delim = delim;
    handle->m_escape = escape;

    handle->m_file = fopen(file_name, "r");

    fseek(handle->m_file, 0, SEEK_END);
    size_t size = ftell(handle->m_file);
    size += 1;
    fseek(handle->m_file, 0, SEEK_SET);

    char* buffer = malloc(sizeof(char) * size);

    fread(buffer, sizeof(char), size - 1, handle->m_file);

    buffer[size] = '\0';

    handle->m_contents = strdup(buffer);

    return handle;
}

SimpCSV_Handle* simpcsv_parse_file(SimpCSV_Handle* handle)
{
    size_t cs = _SIMPCSV_CELL_INCREMENT, bs = _SIMPCSV_CHAR_INCREMENT, ci = 0, bi = 0;
    size_t delim_c = 1, escape_c = 1;

    handle->m_cell = malloc(cs);
    char* buffer = malloc(bs);

    // slow
    for (size_t i = 0; i < strlen(handle->m_contents); i++)
    {
        if (handle->m_contents[i] == handle->m_quote)
        {
            do
            {
                _SIMPCSV_CHAR_CHECK_IF_RESIZE_NEEDED(buffer,
                                                     bi,
                                                     bs - 1,
                                                     _SIMPCSV_CHAR_INCREMENT);

                buffer[bi++] = handle->m_contents[++i];
            } while (handle->m_contents[i] != handle->m_quote);
        }
        while (handle->m_contents[i] != handle->m_delim  &&
               handle->m_contents[i] != handle->m_escape &&
               handle->m_contents[i] != '\0')
        {
            _SIMPCSV_CHAR_CHECK_IF_RESIZE_NEEDED(buffer,
                                                 bi,
                                                 bs - 1,
                                                 _SIMPCSV_CHAR_INCREMENT);

            buffer[bi++] = handle->m_contents[i++];
        }

        _SIMPCSV_CELL_CHECK_IF_RESIZE_NEEDED(handle->m_cell,
                                             ci * sizeof(_SimpCSV_Cell),
                                             cs, _SIMPCSV_CELL_INCREMENT);
        buffer[bi] = '\0';
        handle->m_cell[ci].m_col = delim_c;
        handle->m_cell[ci].m_row = escape_c;

        if (handle->m_contents[i] == handle->m_delim) delim_c++;
        else if (handle->m_contents[i] == handle->m_escape)
        {
            handle->m_number_of_cols = delim_c; // reassigned every single time - slow
            escape_c++;
            delim_c = 1;
        }

        handle->m_number_of_rows = escape_c; // same thing

        handle->m_cell[ci++].m_data = strdup(buffer);
        bi = 0;
    }

    handle->m_number_of_cells = ci;

    free(buffer);

    return handle;
}

size_t simpcsv_get_number_of_columns(SimpCSV_Handle* handle)
{
    return handle->m_number_of_cols;
}

size_t simpcsv_get_number_of_rows(SimpCSV_Handle* handle)
{
    return handle->m_number_of_rows;
}

void simpcsv_close_file(SimpCSV_Handle* handle)
{
    fclose(handle->m_file);
    free(handle->m_contents);
    free(handle->m_cell);
    free(handle);
}
