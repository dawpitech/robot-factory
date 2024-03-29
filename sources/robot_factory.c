/*
** EPITECH PROJECT, 2024
** robot-factory
** File description:
** robot header
*/

#include <endian.h>
#include <malloc.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "robot_factory.h"
#include "my.h"
#include "toolbox.h"

static
bool is_file_right_type(char const *input_file_name)
{
    return my_strcmp(".s", &input_file_name[NOT_NEG(
        my_strlen(input_file_name)) - 2]) == 0;
}

static
char *get_output_file_name(char *input_file_name)
{
    char *rst;
    char *original_ptr;

    original_ptr = input_file_name;
    if (!is_file_right_type(input_file_name))
        return NULL;
    while (my_strchr(input_file_name, '/') != NULL)
        input_file_name = my_strchr(input_file_name, '/') + 1;
    for (int i = my_strlen(input_file_name); i > 0; i -= 1) {
        if (my_strcmp(&input_file_name[i], ".s") == 0)
            input_file_name[i] = '\0';
    }
    rst = malloc(sizeof(char) * (my_strlen(input_file_name) + 5));
    my_strcpy(rst, input_file_name);
    my_strcat(rst, ".cor");
    free(original_ptr);
    return rst;
}

static
int initialize_assm_header(assm_cfg_t *assm_cfg)
{
    assm_cfg->header = malloc(sizeof(header_t));
    if (assm_cfg->header == NULL)
        return RET_ERROR;
    assm_cfg->header->magic = htobe32(COREWAR_EXEC_MAGIC);
    assm_cfg->header->prog_size = 0;
    return RET_VALID;
}

static
int initialize_assm(assm_cfg_t *assm_cfg, char *input_file_name)
{
    char *output_file_name;

    output_file_name = get_output_file_name(my_strdup(input_file_name));
    if (output_file_name == NULL)
        return RET_ERROR;
    assm_cfg->output_filename = output_file_name;
    assm_cfg->buffer = NULL;
    assm_cfg->buffer_size = 0;
    return initialize_assm_header(assm_cfg);
}

static
void exit_hook_assm(assm_cfg_t *assm_cfg)
{
    label_t *current = assm_cfg->labels;
    label_t *next_node = NULL;

    while (current != NULL) {
        next_node = current->next;
        free(current->name);
        free(current);
        current = next_node;
    }
    current = assm_cfg->labels_tolink;
    while (current != NULL) {
        next_node = current->next;
        free(current->name);
        free(current);
        current = next_node;
    }
    if (assm_cfg->output_filename)
        free(assm_cfg->output_filename);
    free(assm_cfg->header);
    free(assm_cfg->buffer);
}

static
int create_file(assm_cfg_t *assm_cfg)
{
    assm_cfg->output_file = fopen(assm_cfg->output_filename, "w");
    if (assm_cfg->output_file == NULL) {
        exit_hook_assm(assm_cfg);
        return RET_ERROR;
    }
    return RET_VALID;
}

static
int write_exit(assm_cfg_t *assm_cfg)
{
    write_header_to_output(assm_cfg);
    write_buff_to_output(assm_cfg);
    exit_hook_assm(assm_cfg);
    fclose(assm_cfg->output_file);
    return RET_VALID;
}

int robot_factory(int argc, char **argv)
{
    assm_cfg_t assm_cfg = {0};

    if (argc != 2 || initialize_assm(&assm_cfg, argv[1]) == RET_ERROR)
        return RET_ERROR;
    if (parse_file(argv[1], &assm_cfg) == RET_ERROR) {
        exit_hook_assm(&assm_cfg);
        return RET_ERROR;
    }
    if (assm_cfg.line_nb == 0) {
        exit_hook_assm(&assm_cfg);
        return my_put_stderr("The file is empty.\n");
    }
    if (link_labels(&assm_cfg) == RET_ERROR) {
        exit_hook_assm(&assm_cfg);
        return RET_ERROR;
    }
    if (create_file(&assm_cfg) == RET_ERROR)
        return RET_ERROR;
    return write_exit(&assm_cfg);
}
