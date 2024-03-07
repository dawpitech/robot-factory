/*
** EPITECH PROJECT, 2024
** link
** File description:
** title
*/

#include "op.h"
#include "robot_factory.h"
#include <stdio.h>
#include <stdlib.h>
#include "my.h"

static
int invalid_label(char c, char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
        if (c != str[i])
            return 1;
    return 0;
}

static
int is_good_label(char *label)
{
    for (int i = 0; label[i] != '\0'; i++) {
        if (invalid_label(label[i], LABEL_CHARS))
            return RET_ERROR;
    }
    return RET_VALID;
}

int add_to_label(int address, char *label, label_t **node, int where)
{
    label_t *new_node = NULL;
    char *tmp = label;

    while (*label == '%' || *label == ':')
        label++;
    while (label[my_strlen(label) - 1] == ':' ||
        label[my_strlen(label) - 1] == '\n' ||
        label[my_strlen(label) - 1] == '\t' ||
        label[my_strlen(label) - 1] == ' ')
        label[my_strlen(label) - 1] = '\0';
    new_node = malloc(sizeof(label_t));
    new_node->name = my_strdup(label);
    new_node->address = address;
    new_node->where = where;
    new_node->next = *node;
    *node = new_node;
    free(tmp);
    return is_good_label(label);
}

static
void write_bytes_at(int num, int byte_nb, assm_cfg_t *assm_cfg,
    unsigned int addr)
{
    char extracted_byte = 0;

    if (byte_nb == 2)
        num = htobe16(num);
    if (byte_nb == 4)
        num = htobe32(num);
    if (byte_nb < 1)
        return;
    for (int i = 0; i < byte_nb; i++) {
        extracted_byte = (num >> (8 * i)) & 0xFF;
        assm_cfg->buffer[addr + i] = extracted_byte;
    }
}

static
void iterate_labels(assm_cfg_t *assm_cfg, int *ret)
{
    unsigned int addr = 0;
    int size = assm_cfg->labels_tolink->is_idx ? IND_SIZE : DIR_SIZE;

    while (assm_cfg->labels != NULL && assm_cfg->labels_tolink != NULL &&
        assm_cfg->labels_tolink->name != NULL &&
        assm_cfg->labels->name != NULL) {
        if (my_strcmp(assm_cfg->labels_tolink->name,
            assm_cfg->labels->name) == 0) {
            *ret = 1;
            addr = assm_cfg->labels->address -
                assm_cfg->labels_tolink->address + 1;
            write_bytes_at(addr,
        size, assm_cfg, assm_cfg->labels_tolink->where);
        }
        assm_cfg->labels = assm_cfg->labels->next;
    }
}

int link_labels(assm_cfg_t *assm_cfg)
{
    label_t *tmp = assm_cfg->labels;
    int ret = 0;

    while (assm_cfg->labels_tolink != NULL) {
        iterate_labels(assm_cfg, &ret);
        if (ret == 0)
            return my_put_stderr("Undefined label.\n");
        ret = 0;
        assm_cfg->labels = tmp;
        assm_cfg->labels_tolink = assm_cfg->labels_tolink->next;
    }
    return RET_VALID;
}
