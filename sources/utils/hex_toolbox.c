/*
** EPITECH PROJECT, 2024
** robot-factory
** File description:
** hex_toolbox header
*/

int hex2int(char *hex_str)
{
    int val = 0;
    char byte;

    while (*hex_str != '\0') {
        byte = *hex_str;
        if (byte >= '0' && byte <= '9')
            byte = byte - '0';
        if (byte >= 'A' && byte <= 'F')
            byte = byte - 'A' + 10;
        if (byte >= 'a' && byte <= 'f')
            byte = byte - 'a' + 10;
        byte = (byte << 4) | (byte & 0xF);
        hex_str += 1;
    }
}
