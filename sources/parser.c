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
int classify_arg(arg_list_t *arguments, char *arg, op_t *op, int args)
{
    int value = 0;

    if (arg[0] == 'r' && my_strlen(arg) >= 2) {
        value = my_getnbr(&arg[1]);
        if (value > MAX_ARGS_NUMBER || value < 0)
            return RET_ERROR;
        arguments->type = REGISTER;
    }
    if (arg[0] == DIRECT_CHAR && my_strlen(arg) >= 2) {
        value = my_getnbr(&arg[1]);
        arguments->type = DIRECT;
    }
    if (my_strlen(arg) >= 2) {
        value = my_getnbr(arg);
        arguments->type = INDIRECT;
    }
    return !(op->type[args] & arguments->type);
}

static
void temporary_fix_for_cs(arg_list_t *curr_args, char *input)
{
    curr_args->data = my_strdup(input);
    curr_args->next = malloc(sizeof(arg_list_t));
}

static
int parse_args(char *input, op_t *op)
{
    char *start_of_args = input + my_strlen(input) + 1;
    char *ptr = my_strtok(start_of_args, SEPARATOR_CHAR);
    int args = 0;
    arg_list_t *arguments = malloc(sizeof(arg_list_t));
    arg_list_t *curr_args = arguments;

    while (ptr != NULL) {
        if ((args > op->nbr_args) || curr_args == NULL)
            return RET_ERROR;
        while (*ptr == '\t' || *ptr == ' ')
            ptr++;
        my_memset(curr_args, 0, sizeof(arg_list_t));
        if (*ptr != COMMENT_CHAR)
            classify_arg(curr_args, ptr, op, args);
        temporary_fix_for_cs(curr_args, input);
        curr_args = curr_args->next;
        ptr = my_strtok(NULL, SEPARATOR_CHAR);
        args++;
    }
    return (args < op->nbr_args) ? RET_ERROR : RET_VALID;
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
