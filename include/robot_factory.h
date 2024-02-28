/*
** EPITECH PROJECT, 2024
** robot-factory
** File description:
** robot header
*/

#ifndef ROBOT_FACTORY_ROBOT_FACTORY_H
    #define ROBOT_FACTORY_ROBOT_FACTORY_H
    #include "op.h"
    #include <bits/types/FILE.h>
    #define RET_ERROR 84
    #define RET_VALID 0
    #define MAGIC_BYTES (-209458688)
typedef enum {
    NA,
    COMMENT,
    NAME
} raw_str_type_e;
typedef struct {
    char *argv;
    int argc;
} line_t;
typedef enum {
    DIRECT = 0b10,
    INDIRECT = 0b11,
    REGISTER = 0b01,
    PADDING = 0b00
} arg_type_e;
typedef struct arg_list_s {
    struct arg_list_s *next;
    char *data;
    arg_type_e type;
} arg_list_t;
typedef struct {
    char *buffer;
    unsigned int buffer_size;
    FILE *output_file;
    header_t *header;
} assm_cfg_t;
int robot_factory(int argc, char **argv);
int parse_file(char *file_path, assm_cfg_t *assm_cfg);
int write_byte(char byte, assm_cfg_t *assm_cfg);
int write_to_header(char *str, assm_cfg_t *assm_cfg, raw_str_type_e type);
int write_buff_to_output(assm_cfg_t *assm_cfg);
int write_header_to_output(assm_cfg_t *assm_cfg);
#endif //ROBOT_FACTORY_ROBOT_FACTORY_H
