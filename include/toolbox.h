/*
** EPITECH PROJECT, 2024
** robot-factory
** File description:
** toolbox header
*/

#ifndef ROBOT_FACTORY_TOOLBOX_H
    #define ROBOT_FACTORY_TOOLBOX_H
char *skip_label(char *ptr);
void my_memcpy(void *dest, void *src, size_t size);
char *my_strchr(char const *str, char token);
char *extract_from_quotes(char const *input);
char *my_strtok(char *str, char separator);
#endif //ROBOT_FACTORY_TOOLBOX_H
