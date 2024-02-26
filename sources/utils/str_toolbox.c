/*
** EPITECH PROJECT, 2024
** robot-factory
** File description:
** str_toolbox header
*/

#include <malloc.h>
#include <stddef.h>

#include "toolbox.h"
#include "my.h"

char *skip_label(char *ptr)
{
    while ((*ptr != '\t' && *ptr != ' ')
        && *ptr != '\0')
        ptr++;
    while (*ptr == '\t' || *ptr == ' ')
        ptr++;
    return ptr;
}

char *my_strchr(char const *str, char token)
{
    char *output = str;

    while (*output != token && *output != '\0')
        output += 1;
    return *output == '\0' ? NULL : output;
}

char *extract_from_quotes(char const *input)
{
    char *start = my_strchr(input, '"');
    char *end;
    char *rst;
    size_t length;

    if (start == NULL)
        return NULL;
    start += 1;
    end = my_strchr(start, '"');
    if (end == NULL)
        return NULL;
    length = end - start;
    rst = malloc(length + 1);
    if (rst == NULL)
        return NULL;
    my_strncpy(rst, start, (int) length);
    rst[length] = '\0';
    return rst;
}
