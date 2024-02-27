# How to compile instructions
[uwu](https://www.youtube.com/watch?v=dQw4w9WgXcQ)

## The structure of the bytecode

- The [opcode](#opcode)
- For any instruction other than `live`, `zjmp`, `fork` and `lfork`, the [coding byte](#coding-byte)
- The [parameters](#the-parameters)

## Opcode
In `op.c` you can get the `char code` field, it will be the value of our opcode. For example, `sti` will be `0x0b`.

## Coding byte
The coding byte is a single byte created from the types of the arguments.<br>

- 01 if register
- 10 if direct
- 11 if indirect
- 00 if no arguments

Let's take `sti r2, 23, %34` as an example.<br>
r2 is a register so we add 01b to the byte.<br>
23 is an indirect so we add 11b to the byte.<br>
%34 is a direct so we add 10b to the byte.<br>
As there is only 3 arguments, we will add 00b to the byte to compensate for.<br>

In the end we get `01 11 10 00` which is `0x78`. We have our coding byte.

## The parameters
The size of each arguments is defined in the `op.h` file. Here an argument is 1 byte, a direct 4 bytes and an indirect 2 bytes.<br>

Taking the example from earlier (`sti r2, 23, %34`), we can do the following :<br>
- Add `0x02` because `r2` is the SECOND register and is 1 byte long
- Add `0x00 0x17` because `0x17` is `23` and indirects are 2 bytes long
- Add `0x00 0x00 0x00 0x22` because `0x22` is `34` and directs are 4 bytes long

## The final bytecode
So, following the previous sections, `sti r2, 23, %34` will compile to:<br>

`0x0b 0x78 0x02 0x00 0x17 0x00 0x00 0x00 0x22`<br>

- Byte 1 is opcode
- Byte 2 is the coding byte
- Byte 3 is `r2`
- Bytes 4-5 are `23`
- Bytes 6-9 are `%34`
