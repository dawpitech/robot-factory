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
    if (ptr[0] == '.')
        return ptr;
    while ((*ptr != '\t' && *ptr != ' ') && *ptr != '\0')
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

char *my_strtok(char *str, char separator)
{
    static char *current_str = NULL;
    char *begin_of_str;

    if (str != NULL)
        current_str = str;
    if (current_str == NULL || *current_str == '\0')
        return NULL;
    while (*current_str == separator)
        current_str += 1;
    begin_of_str = current_str;
    while (*current_str != '\0' && *current_str != separator)
        current_str += 1;
    if (*current_str != '\0') {
        *current_str = '\0';
        current_str += 1;
    }
    return begin_of_str;
}
