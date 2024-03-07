/*
** EPITECH PROJECT, 2024
** robot-factory
** File description:
** str_toolbox header
*/

#include <malloc.h>
#include <stddef.h>

#include "robot_factory.h"
#include "toolbox.h"
#include "my.h"

static
char *delete_spaces(char *ptr)
{
    while (*ptr == ' ' || *ptr == '\t')
        ptr++;
    return ptr;
}

char *strip_label(char *ptr, char **label_str)
{
    char *end = NULL;

    ptr = delete_spaces(ptr);
    if (*ptr == '\0') {
        *label_str = NULL;
        return ptr;
    }
    end = ptr;
    while (*end != '\0' && *end != ':' && *end != ' ' && *end != '\t')
        end++;
    if (*end == ':') {
        *label_str = (char *)malloc((end - ptr + 1) * sizeof(char));
        my_strncpy(*label_str, ptr, end - ptr);
        (*label_str)[end - ptr] = '\0';
        ptr = end + 1;
        ptr = delete_spaces(ptr);
    } else {
        *label_str = NULL;
    }
    return ptr;
}

char *my_strchr(char const *str, char token)
{
    char *output = (char *)&(*str);

    while (*output != token && *output != '\0')
        output += 1;
    return *output == '\0' ? NULL : output;
}

void remove_commentaries(char *ptr)
{
    for (; *ptr != '\0'; ptr++)
        if (*ptr == '#')
            *ptr = '\0';
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
