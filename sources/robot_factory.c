/*
** EPITECH PROJECT, 2024
** robot-factory
** File description:
** robot header
*/

#include <endian.h>
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>

#include "robot_factory.h"

static
int initialize_assm(assm_cfg_t *assm_cfg)
{
    assm_cfg->header = malloc(sizeof(header_t));
    if (assm_cfg->header == NULL)
        return RET_ERROR;
    assm_cfg->header->magic = htobe32(COREWAR_EXEC_MAGIC);
    assm_cfg->header->prog_size = 0;
    assm_cfg->output_file = fopen("output.cor", "w");
    assm_cfg->buffer = NULL;
    assm_cfg->buffer_size = 0;
    return RET_VALID;
}

int robot_factory(int argc, char **argv)
{
    assm_cfg_t assm_cfg = {0};

    if (argc != 2 || initialize_assm(&assm_cfg) == RET_ERROR)
        return RET_ERROR;
    parse_file(argv[1], &assm_cfg);
    write_header_to_output(&assm_cfg);
    write_buff_to_output(&assm_cfg);
    return RET_VALID;
}
