/*
** EPITECH PROJECT, 2024
** robot-factory
** File description:
** toolbox header
*/

#ifndef ROBOT_FACTORY_TOOLBOX_H
    #define ROBOT_FACTORY_TOOLBOX_H
char *skip_label(char *ptr);
int hex2int(char *hex_str);
void my_memcpy(void *dest, void *src, size_t size);
char *my_strchr(char const *str, char token);
char *extract_from_quotes(char const *input);
#endif //ROBOT_FACTORY_TOOLBOX_H
