#ifndef CHIP8_H
#define CHIP8_H

#include "config.h"
#include "chip8memory.h"
#include "chip8stack.h"
#include "chip8registers.h"
#include "chip8keyboard.h"
#include "chip8screen.h"

struct Chip8
{
    struct Chip8Memory memory;
    struct Chip8Stack stack;
    struct Chip8Registers registers;
    struct Chip8Keyboard keyboard;
    struct Chip8Screen screen;
};

void initChip8(struct Chip8 *chip8);

#endif