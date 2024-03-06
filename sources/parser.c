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
    static int names = 0;
    static int comments = 0;

    if (my_strncmp(input, ".name", 5) == 0) {
        if (my_strlen(input) - 5 > 128)
            return my_put_stderr("The program name is too long.\n");
        temp = extract_from_quotes(input);
        ++names;
        write_to_header(temp, assm_cfg, NAME);
    }
    if (my_strncmp(input, ".comment", 8) == 0) {
        if (my_strlen(input) - 8 > 2048)
            return my_put_stderr("The comment is too long.\n");
        temp = extract_from_quotes(input);
        ++comments;
        write_to_header(temp, assm_cfg, COMMENT);
    }
    if (temp != NULL)
        free(temp);
    if (names > 1 || comments > 1)
        return my_put_stderr("Name or comment defined more than once.\n");
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
    return RET_VALID;
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
        return RET_VALID;
    for (int i = 0; op_tab[i].comment != 0; i += 1)
        if (my_strcmp(op_tab[i].mnemonique, assm_cfg->line->command) == 0)
            return parse_line(assm_cfg->line->command, &op_tab[i], assm_cfg);
    return RET_VALID;
}

static int free_fclose_return(char *line, FILE *stream, int ret)
{
    free(line);
    fclose(stream);
    return ret;
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
    for (; getline(&line, &buff_value, stream) > 0;) {
        if (line[0] == '\n' || line[0] == '\0')
            continue;
        if (tokenize_line(line, assm_cfg) == RET_ERROR)
            return free_fclose_return(line, stream, RET_ERROR);
        if (assm_cfg->line->label)
            free(assm_cfg->line->label);
    }
    return free_fclose_return(line, stream, RET_VALID);
}
