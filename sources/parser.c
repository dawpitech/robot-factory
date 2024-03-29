/*
** EPITECH PROJECT, 2024
** robot-factory
** File description:
** parser header
*/

#include <malloc.h>
#include <stddef.h>
#include <stdio.h>

#include "op.h"
#include "robot_factory.h"
#include "my.h"
#include "toolbox.h"

static
int check_temp(char *temp, assm_cfg_t *assm_cfg, raw_str_type_e type,
    char *input)
{
    char *input_cp = my_strdup(input);
    char *after_db_quotes = input_cp;

    if (temp == NULL) {
        free(temp);
        return my_put_stderr("No name/comment specified.\n");
    }
    for (int i = 2; i > 0;) {
        if (*after_db_quotes == '\"')
            i--;
        after_db_quotes++;
    }
    if (*after_db_quotes != '\0' && *after_db_quotes != '\n') {
        free(temp);
        free(input_cp);
        return my_put_stderr("Char(s) after name/comment.\n");
    }
    write_to_header(temp, assm_cfg, type);
    free(temp);
    return RET_VALID;
}

static
int do_for_comment(char *input, assm_cfg_t *assm_cfg, raw_str_type_e type)
{
    char *temp = NULL;
    static int names = 0;
    static int comments = 0;

    if (type == NAME) {
        if (my_strlen(input) - my_strlen(NAME_CMD_STRING) > PROG_NAME_LENGTH)
            return (int) my_put_stderr("The program name is too long.\n");
        ++names;
    } else {
        if (my_strlen(input) - my_strlen(COMMENT_CMD_STRING) > COMMENT_LENGTH)
            return (int) my_put_stderr("The comment is too long.\n");
        ++comments;
    }
    temp = extract_from_quotes(input);
    if (check_temp(temp, assm_cfg, type, input) == RET_ERROR)
        return RET_ERROR;
    if (names > 1 || comments > 1)
        return my_put_stderr("Name or comment defined more than once.\n");
    return RET_VALID;
}

static
int check_for_comment(char *input, assm_cfg_t *assm_cfg)
{
    if (my_strncmp(input, NAME_CMD_STRING, my_strlen(NAME_CMD_STRING)) == 0) {
        if (assm_cfg->line->label)
            return my_put_stderr("A label can't point to a .name.\n");
        if (assm_cfg->line_nb == 0 || assm_cfg->buffer_size == 0)
            return do_for_comment(input, assm_cfg, NAME);
        return my_put_stderr(".name must be the first line.\n");
    }
    if (my_strncmp(input, COMMENT_CMD_STRING,
        my_strlen(COMMENT_CMD_STRING)) == 0) {
        if (assm_cfg->line->label)
            return my_put_stderr("A label can't point to a .comment.\n");
        if (assm_cfg->header->prog_name[0] != '\0')
            return do_for_comment(input, assm_cfg, COMMENT);
        return my_put_stderr(".comment must be the second line.\n");
    }
    return RET_VALID;
}

static
void classify_arg(arg_t *arg, char *raw_arg)
{
    switch (raw_arg[0]) {
        case REGISTER_CHAR:
            arg->type = REGISTER;
            arg->type_e = T_REG;
            break;
        case DIRECT_CHAR:
            arg->type = DIRECT;
            arg->type_e = T_DIR;
            break;
        default:
            arg->type = INDIRECT;
            arg->type_e = T_IND;
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
    return compile_line(op, args, assm_cfg);
}

static int is_blank(char *ptr)
{
    if (ptr == NULL)
        return 1;
    while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n')
        ptr++;
    return *ptr == '\0';
}

static int check_invalid_ins(assm_cfg_t *assm_cfg)
{
    if (assm_cfg->line->command == NULL ||
        my_strcmp(assm_cfg->line->command, ".name") == 0 ||
        my_strcmp(assm_cfg->line->command, ".comment") == 0 ||
        is_blank(assm_cfg->line->command))
        return RET_VALID;
    for (int i = 0; op_tab[i].comment != 0; i += 1)
        if (my_strcmp(op_tab[i].mnemonique, assm_cfg->line->command) == 0)
            return RET_VALID;
    return my_put_stderr("Invalid instruction.\n");
}

static
int tokenize_line(char *input, assm_cfg_t *assm_cfg)
{
    assm_cfg->line->label = NULL;
    remove_commentaries(input);
    assm_cfg->line->command = strip_label(input, &assm_cfg->line->label);
    if (check_for_comment(assm_cfg->line->command, assm_cfg) == RET_ERROR)
        return RET_ERROR;
    if (assm_cfg->line->label)
        if (add_to_label(assm_cfg->buffer_size, assm_cfg->line->label,
            &assm_cfg->labels, assm_cfg->buffer_size) == RET_ERROR)
            return my_put_stderr("Invalid label name.\n");
    assm_cfg->line->command = my_strtok_2(assm_cfg->line->command, " \t");
    if (assm_cfg->line->command == NULL)
        return RET_VALID;
    for (int i = 0; op_tab[i].comment != 0; i += 1)
        if (my_strcmp(op_tab[i].mnemonique, assm_cfg->line->command) == 0)
            return parse_line(assm_cfg->line->command, &op_tab[i], assm_cfg);
    return check_invalid_ins(assm_cfg);
}

static
int free_fclose_return(char *line, FILE *stream, int ret)
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
    for (assm_cfg->line_nb = 0;
        getline(&line, &buff_value, stream) > 0; assm_cfg->line_nb++) {
        if (line[0] == '\n' || line[0] == '\0')
            continue;
        if (tokenize_line(line, assm_cfg) == RET_ERROR)
            return free_fclose_return(line, stream, RET_ERROR);
    }
    return free_fclose_return(line, stream, RET_VALID);
}
