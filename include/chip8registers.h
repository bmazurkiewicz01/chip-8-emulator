#ifndef CHIP8REGISTERS_H
#define CHIP8REGISTERS_H

#include "config.h"

struct Chip8Registers
{
    unsigned char V[CHIP8_REGISTERS_SIZE];
    unsigned short I;
    unsigned char DT;
    unsigned char ST;
    unsigned short PC;
    unsigned char SP;

};


#endif