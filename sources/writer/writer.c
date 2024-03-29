/*
** EPITECH PROJECT, 2024
** robot-factory
** File description:
** writer header
*/

#include <malloc.h>
#include <stddef.h>
#include <sys/types.h>

#include "robot_factory.h"
#include "my.h"
#include "toolbox.h"

int write_buff_to_output(assm_cfg_t *assm_cfg)
{
    fwrite(assm_cfg->buffer, sizeof(char), assm_cfg->buffer_size,
        assm_cfg->output_file);
    return RET_VALID;
}

int write_header_to_output(assm_cfg_t *assm_cfg)
{
    assm_cfg->header->prog_size = htobe32((int) assm_cfg->buffer_size);
    fwrite(assm_cfg->header, sizeof(header_t), 1, assm_cfg->output_file);
    return RET_VALID;
}

int write_byte(char byte, assm_cfg_t *assm_cfg)
{
    char *new_buff = realloc(assm_cfg->buffer, sizeof(char) *
        (assm_cfg->buffer_size + 1));

    if (new_buff == NULL)
        return RET_ERROR;
    assm_cfg->buffer = new_buff;
    assm_cfg->buffer_size += 1;
    assm_cfg->buffer[assm_cfg->buffer_size - 1] = byte;
    return RET_VALID;
}

void write_bytes(long num, int byte_nb, assm_cfg_t *assm_cfg)
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
        write_byte(extracted_byte, assm_cfg);
    }
}

int write_to_header(char *str, assm_cfg_t *assm_cfg, raw_str_type_e type)
{
    char *target;

    if (str == NULL || assm_cfg == NULL)
        return RET_ERROR;
    switch (type) {
        case NAME:
            target = assm_cfg->header->prog_name;
            break;
        case COMMENT:
            target = assm_cfg->header->comment;
            break;
        default:
        case NA:
            return RET_ERROR;
    }
    my_memcpy(target, str, sizeof(char) * (my_strlen(str)));
    return RET_VALID;
}
