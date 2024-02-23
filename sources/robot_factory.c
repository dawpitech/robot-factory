/*
** EPITECH PROJECT, 2024
** robot-factory
** File description:
** robot header
*/

#include <malloc.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "robot_factory.h"
#include "op.h"

static
void analyze_line(char *input, header_t *header)
{
    char *start_of_line = input;
    char *str_val;

    while ((*start_of_line != '\t' && *start_of_line != ' ')
        && *start_of_line != '\0')
        start_of_line++;
    while (*start_of_line == '\t' || *start_of_line == ' ')
        start_of_line++;
    if (strncmp(start_of_line, ".name", 5) == 0) {
        str_val = start_of_line;
        while (*str_val != '"')
            str_val++;
        str_val++;
        memcpy(header->prog_name, str_val, sizeof(char) * (strlen(str_val) - 2));
    }
    if (strncmp(start_of_line, ".comment", 8) == 0) {
        str_val = start_of_line;
        while (*str_val != '"')
            str_val++;
        str_val++;
        memcpy(header->comment, str_val, sizeof(char) * (strlen(str_val) - 2));
    }
    printf("%s", start_of_line);
}

static
void my_parser(char *file_path, header_t *header)
{
    FILE *stream;
    char *line = NULL;
    size_t buff_value = 0;

    stream = fopen(file_path, "r");
    if (stream == NULL)
        return;
    for (; getline(&line, &buff_value, stream) > 0;) {
        analyze_line(line, header);
    }
    free(line);
    fclose(stream);
}

int robot_factory(int argc, char **argv)
{
    FILE *output;
    header_t header = {0};

    header.magic = -209458688;
    header.prog_size = 0;
    if (argc != 2)
        return 84;
    my_parser(argv[1], &header);
    output = fopen("output.cor", "w");
    fwrite(&header, sizeof(header_t), 1, output);
    fclose(output);
    return 0;
}
