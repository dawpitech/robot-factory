/*
** EPITECH PROJECT, 2024
** robot-factory
** File description:
** parser header
*/

#include <assert.h>
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>

#include "robot_factory.h"
#include "my.h"
#include "toolbox.h"

static
int check_for_comment(char *input, assm_cfg_t *assm_cfg)
{
    char *temp = NULL;

    if (my_strncmp(input, ".name", 5) == 0) {
        temp = extract_from_quotes(input);
        write_to_header(temp, assm_cfg, NAME);
    }
    if (my_strncmp(input, ".comment", 8) == 0) {
        temp = extract_from_quotes(input);
        write_to_header(temp, assm_cfg, COMMENT);
    }
    if (temp != NULL) {
        free(temp);
        return RET_VALID;
    }
    return RET_ERROR;
}

static
int classify_arg(char *arg, op_t *op, int args)
{
    args_type_t arg_type = 0;
    int value = 0;

    if (arg[0] == 'r' && my_strlen(arg) >= 2) {
        value = my_getnbr(&arg[1]);
        if (value > MAX_ARGS_NUMBER || value < 0)
            return 1;
        arg_type = T_REG;
    }
    if (arg[0] == DIRECT_CHAR && my_strlen(arg) >= 2) {
        value = my_getnbr(&arg[1]);
        arg_type = T_DIR;
    }
    if (my_strlen(arg) >= 2) {
        value = my_getnbr(arg);
        arg_type = T_IND;
    }
    if (op->type[args] & arg_type)
        return 0;
    return 1;
}

static
int parse_args(char *input, op_t *op)
{
    char *start_of_args = input + my_strlen(input) + 1;
    int cb = 0;
    char *ptr = my_strtok(start_of_args, SEPARATOR_CHAR);
    int args = 0;

    while (ptr != NULL) {
        while (*ptr == '\t' || *ptr == ' ')
            ptr++;
        if (*ptr != COMMENT_CHAR)
            classify_arg(ptr, op, args);
        ptr = my_strtok(NULL, SEPARATOR_CHAR);
        args++;
        if (args > op->nbr_args)
            return 1;
    }
    return args < op->nbr_args;
}

static
void tokenize_line(char *input, assm_cfg_t *assm_cfg)
{
    char *start_of_line = input;
    char *command;
    int i = 0;

    start_of_line = skip_label(start_of_line);
    if (check_for_comment(start_of_line, assm_cfg) == RET_VALID)
        return;
    command = my_strtok(start_of_line, ' ');
    for (i = 0; op_tab[i].comment != 0; i += 1)
        if (my_strcmp(op_tab[i].mnemonique, command) == 0)
            write_byte(op_tab[i].code, assm_cfg);
    parse_args(start_of_line, &op_tab[i]);
}

int parse_file(char *file_path, assm_cfg_t *assm_cfg)
{
    FILE *stream;
    char *line = NULL;
    size_t buff_value = 0;

    stream = fopen(file_path, "r");
    if (stream == NULL)
        return RET_ERROR;
    for (; getline(&line, &buff_value, stream) > 0;)
        if (line[0] != '\n' && line[0] != '\0')
            tokenize_line(line, assm_cfg);
    free(line);
    fclose(stream);
    return RET_VALID;
}
