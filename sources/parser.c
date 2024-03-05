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
        return RET_ERROR;
    }
    return RET_VALID;
}

static
void classify_arg(arg_t *arg, char *raw_arg)
{
    switch (raw_arg[0]) {
        case REGISTER_CHAR:
            arg->type = REGISTER;
            break;
        case DIRECT_CHAR:
            arg->type = DIRECT;
            break;
        default:
            arg->type = INDIRECT;
            break;
    }
    arg->data = raw_arg;
}

static
int parse_line(char *input, op_t *op, assm_cfg_t *assm_cfg)
{
    char *ptr = my_strtok(input + my_strlen(input) + 1, SEPARATOR_CHAR);
    arg_t args[MAX_ARGS_NUMBER] = {0};

    for (int index = 0; index < MAX_ARGS_NUMBER; index += 1) {
        while (ptr != NULL && (*ptr == ' ' || *ptr == '\t'))
            ptr++;
        if (ptr == NULL) {
            args[index].type = PADDING;
            args[index].data = NULL;
        } else {
            classify_arg(&args[index], ptr);
        }
        ptr = my_strtok(NULL, SEPARATOR_CHAR);
    }
    compile_line(op, args, assm_cfg);
    return 0;
}

static
int tokenize_line(char *input, assm_cfg_t *assm_cfg)
{
    assm_cfg->line->label = NULL;
    remove_commentaries(input);
    assm_cfg->line->command = strip_label(input, &assm_cfg->line->label);
    if (assm_cfg->line->label)
        add_to_label(assm_cfg->buffer_size, assm_cfg->line->label,
            &assm_cfg->labels);
    if (check_for_comment(assm_cfg->line->command, assm_cfg) == RET_ERROR)
        return RET_ERROR;
    assm_cfg->line->command = my_strtok(assm_cfg->line->command, ' ');
    if (assm_cfg->line->command == NULL)
        return assm_cfg->line->label ? RET_VALID : RET_ERROR;
    for (int i = 0; op_tab[i].comment != 0; i += 1)
        if (my_strcmp(op_tab[i].mnemonique, assm_cfg->line->command) == 0)
            return parse_line(assm_cfg->line->command, &op_tab[i], assm_cfg);
    return RET_VALID;
}

int parse_file(char *file_path, assm_cfg_t *assm_cfg)
{
    FILE *stream;
    char *line = NULL;
    size_t buff_value = 0;
    line_t line_buff = {0};

    stream = fopen(file_path, "r");
    assm_cfg->line = &line_buff;
    if (stream == NULL)
        return RET_ERROR;
    for (; getline(&line, &buff_value, stream) > 0;)
        if (line[0] != '\n' && line[0] != '\0')
            tokenize_line(line, assm_cfg);
    free(line);
    fclose(stream);
    return RET_VALID;
}
