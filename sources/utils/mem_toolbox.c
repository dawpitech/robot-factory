/*
** EPITECH PROJECT, 2024
** minishell
** File description:
** mem_toolbox header
*/

#include <stddef.h>

#include "toolbox.h"

void my_memcpy(void *dest, void *src, size_t size)
{
    char *cdest = (char *) dest;
    char *csrc = (char *) src;

    for (size_t i = 0; i < size; i += 1)
        cdest[i] = csrc[i];
}
