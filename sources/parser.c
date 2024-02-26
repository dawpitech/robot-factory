/*
** EPITECH PROJECT, 2024
** robot-factory
** File description:
** parser header
*/

#include <malloc.h>
#include <stddef.h>
#include <stdio.h>

#include "robot_factory.h"
#include "my.h"
#include "toolbox.h"

static
void analyze_line(char *input, assm_cfg_t *assm_cfg)
{
    char *start_of_line = input;
    char *temp = NULL;

    start_of_line = skip_label(start_of_line);
    if (my_strncmp(start_of_line, ".name", 5) == 0) {
        temp = extract_from_quotes(start_of_line);
        write_to_header(temp, assm_cfg, NAME);
    }
    if (my_strncmp(start_of_line, ".comment", 8) == 0) {
        temp = extract_from_quotes(start_of_line);
        write_to_header(temp, assm_cfg, COMMENT);
    }
    if (temp != NULL)
        free(temp);
    printf("%s", start_of_line);
}

int parse_file(char *file_path, assm_cfg_t *assm_cfg)
{
    FILE *stream;
    char *line = NULL;
    size_t buff_value = 0;

    stream = fopen(file_path, "r");
    if (stream == NULL)
        return RET_ERROR;
    for (; getline(&line, &buff_value, stream) > 0;) {
        analyze_line(line, assm_cfg);
    }
    free(line);
    fclose(stream);
    return RET_VALID;
}
