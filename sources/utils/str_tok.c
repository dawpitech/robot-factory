/*
** EPITECH PROJECT, 2024
** strtok
** File description:
** title
*/

#include <stddef.h>
#include "toolbox.h"

static
int is_separator(char str, char *sep)
{
    for (int i = 0; sep[i] != '\0'; i++)
        if (str == sep[i])
            return 1;
    return 0;
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

char *my_strtok_2(char *str, char *separator)
{
    static char *current_str = NULL;
    char *begin_of_str;

    if (str != NULL)
        current_str = str;
    if (current_str == NULL || *current_str == '\0')
        return NULL;
    while (is_separator(*current_str, separator))
        current_str += 1;
    begin_of_str = current_str;
    while (*current_str != '\0' && !is_separator(*current_str, separator))
        current_str += 1;
    if (*current_str != '\0') {
        *current_str = '\0';
        current_str += 1;
    }
    return begin_of_str;
}
