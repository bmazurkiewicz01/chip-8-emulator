#include <stdio.h>
#include <memory.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "SDL.h"
#include "chip8.h"

const char chip8DefaultCharacterSet[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    0x90, 0x90, 0xF0, 0x10, 0x10,
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    0xF0, 0x10, 0x20, 0x40, 0x40,
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    0xF0, 0x90, 0xF0, 0x80, 0x80};

void initChip8(struct Chip8 *chip8)
{
    memset(chip8, 0, sizeof(struct Chip8));
    memcpy(&chip8->memory.memory, chip8DefaultCharacterSet, sizeof(chip8DefaultCharacterSet));
}

void loadChip8(struct Chip8 *chip8, const char *buffer, size_t bufferSize)
{
    if ((bufferSize + CHIP8_LOAD_ADDRESS) >= CHIP8_MEMORY_SIZE)
    {
        printf("Error: Buffer size is not within the bounds.\n");
    }
    else
    {
        memcpy(&chip8->memory.memory[CHIP8_LOAD_ADDRESS], buffer, bufferSize);
        chip8->registers.PC = CHIP8_LOAD_ADDRESS;
    }
}

static char waitForKeyPress(struct Chip8 *chip8)
{
    SDL_Event event;
    while (SDL_WaitEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            char key = mapKey(&chip8->keyboard, event.key.keysym.sym);
            if (key != -1)
            {
                return key;
            }
        }
    }
    return -1;
}

static void execExtendedChip8(struct Chip8 *chip8, unsigned short opcode)
{
    unsigned short addr = opcode & 0x0FFF;
    unsigned short kk = opcode & 0x00FF;
    unsigned char x = (opcode >> 8) & 0x000F;
    unsigned char y = (opcode >> 4) & 0x000F;
    unsigned char n = opcode & 0x000F;

    unsigned char charResult = 0;
    unsigned short shortResult = 0;

    switch (opcode & 0xF000)
    {
    // Jumps to location addr
    case 0x1000:
        chip8->registers.PC = addr;
        break;

    // Calls subroutine at addr
    case 0x2000:
        pushStack(chip8, chip8->registers.PC);
        chip8->registers.PC = addr;
        break;

    // Skips next instruction if register indexed with the number x is EQUAL to kk
    case 0x3000:
        if (chip8->registers.V[x] == kk)
        {
            chip8->registers.PC += 2;
        }
        break;

    // Skips next instruction if register indexed with the number x is NOT EQUAL to kk
    case 0x4000:
        if (chip8->registers.V[x] != kk)
        {
            chip8->registers.PC += 2;
        }
        break;

    // Skips next insctruction if register indexed with the number x is EQUAL to register indexed with the number y
    case 0x5000:
        if (chip8->registers.V[x] == chip8->registers.V[y])
        {
            chip8->registers.PC += 2;
        }
        break;

    // Puts the value kk to the register indexed with the number x
    case 0x6000:
        chip8->registers.V[x] = kk;
        break;

    // Adds the value kk to the value of register indexed with the number x
    case 0x7000:
        chip8->registers.V[x] += kk;
        break;

    // It is more complex command and it handles nine register operations
    case 0x8000:
    {
        switch (opcode & 0x000F)
        {
        // Sets register indexed with the number x to the value of register indexed with the number y
        case 0x00:
            chip8->registers.V[x] = chip8->registers.V[y];
            break;

        // Performs a bitwise OR on the values of register indexed with the number x and register indexed with the number y
        // Then stores the result of the operation in the register indexed with the number x
        case 0x01:
            charResult = chip8->registers.V[x] | chip8->registers.V[y];
            chip8->registers.V[x] = charResult;
            break;

        // Performs a bitwise AND on the values of register indexed with the number x and register indexed with the number y
        // Then stores the result of the operation in the register indexed with the number x
        case 0x02:
            charResult = chip8->registers.V[x] & chip8->registers.V[y];
            chip8->registers.V[x] = charResult;
            break;

        // Performs a bitwise XOR on the values of register indexed with the number x and register indexed with the number y
        // Then stores the result of the operation in the register indexed with the number x
        case 0x03:
            charResult = chip8->registers.V[x] ^ chip8->registers.V[y];
            chip8->registers.V[x] = charResult;
            break;

        // Adds the value of the register indexed with the number y to the register indexed with the number x
        // If the result of the sum operation is greater than 8 bits register indexed with number 0x0F is set to 1, otherwise 0.
        case 0x04:
            shortResult = chip8->registers.V[x] + chip8->registers.V[y];
            chip8->registers.V[0x0F] = shortResult > 0xFF;
            chip8->registers.V[x] = shortResult;
            break;

        // Substracts the value of the register indexed with the number y from the value of the register indexed with the number x
        // If the value of the register indexed with the number x is greater than the register indexed with the number y
        // Then the register indexed with the number 0x0F is set to 1, otherwise 0
        case 0x05:
            chip8->registers.V[0x0F] = chip8->registers.V[x] > chip8->registers.V[y];
            chip8->registers.V[x] -= chip8->registers.V[y];
            break;

        // If the least-significant bit of register indexed with the number x is 1, then the value of the register indexed with the number 0x0F is set to 1, otherwise 0
        // Then the value of the register indexed with the number x is divided by 2
        case 0x06:
            chip8->registers.V[0x0F] = chip8->registers.V[x] & 0x01;
            chip8->registers.V[x] /= 2;
            break;

        // If the value of the register indexed with the number y is greater than the value of the register indexed with the number x
        // Then the register indexed with the number 0x0F is set to 1, otherwise 0
        // Then the value of the register indexed with the number x is subtracted from the value of the register indexed with the number y,
        // and the results stored in the register indexed with the number x
        case 0x07:
            chip8->registers.V[0x0F] = chip8->registers.V[y] > chip8->registers.V[x];
            chip8->registers.V[x] = chip8->registers.V[y] - chip8->registers.V[x];
            break;

        // If the most-significant bit of register indexed with the number x is 1, then the register indexed with the number 0x0F is set to 1, otherwise to 0
        // Then register indexed with the number x is multiplied by 2
        case 0x0E:
            chip8->registers.V[0x0F] = chip8->registers.V[x] & 0x80; // 0x80 is equal to 0b10000000
            chip8->registers.V[x] *= 2;
            break;
        }
        break;
    }

    // Skips the next instruction if the value of the register indexed by the number x is NOT EQUAL to value of the register indexed by the number y
    case 0x9000:
        if (chip8->registers.V[x] != chip8->registers.V[y])
        {
            chip8->registers.PC += 2;
        }
        break;

    // Sets the I register to the value addr
    case 0xA000:
        chip8->registers.I = addr;
        break;

    // Jumps to location calculated by the sum of the addr value and value of the register indexed by the number 0x00
    case 0xB000:
        chip8->registers.PC = addr + chip8->registers.V[0x00];
        break;

    // Sets the register indexed by the number x to a result of the bitwiste AND operation between random byte and kk
    case 0xC000:
        srand(clock());
        chip8->registers.V[x] = (rand() % 255) & kk;
        break;

    // Displays n-byte starting at memory location I at (register indexed by the number x, register indexed by the number y)
    // If this causes any pixels to be erased, register indexed by the number 0x0F is set to 1, otherwise 0
    case 0xD000:
        const char *sprite = (const char *)&chip8->memory.memory[chip8->registers.I];
        chip8->registers.V[0x0F] = drawSprite(&chip8->screen, chip8->registers.V[x], chip8->registers.V[y], sprite, n);
        break;

    // It is more complex command and it contains 2 keyboard operations
    case 0xE000:
    {
        switch (opcode & 0x00FF)
        {
        // Skips the next instruction if key with the value of the register indexed by the number x is pressed.
        case 0x9E:
            if (isKeyDown(&chip8->keyboard, chip8->registers.V[x]))
            {
                chip8->registers.PC += 2;
            }
            break;

        // Skips the next instruction if key with the value of the register indexed by the number x is NOT pressed.
        case 0xA1:
            if (!isKeyDown(&chip8->keyboard, chip8->registers.V[x]))
            {
                chip8->registers.PC += 2;
            }
            break;
        }
        break;
    }

    // It is more complex command and it contains X operations
    case 0xF000:
    {
        switch (opcode & 0x00FF)
        {
        // Sets the register indexed by the number x to delay timer value.
        case 0x07:
            chip8->registers.V[x] = chip8->registers.DT;
            break;

        // Waits for a key press, then stores the value of the key in the register indexed by the number x
        case 0x0A:
            char key = waitForKeyPress(chip8);
            printf("%c\n", key);
            chip8->registers.V[x] = key;
            break;

        // Sets delay timer to the register indexed by the number x
        case 0x15:
            chip8->registers.DT = chip8->registers.V[x];
            break;

        // Sets sound timer to the register indexed by the number x
        case 0x18:
            chip8->registers.ST = chip8->registers.V[x];
            break;

        // Sets the I register to the sum of the value of I register and the value of the register indexed by the number x
        case 0x1E:
            chip8->registers.I += chip8->registers.V[x];
            break;

        // Sets the I register to the location of sprite for digit register indexed by the number x
        case 0x29:
            chip8->registers.I = chip8->registers.V[x] * CHIP8_DEFAULT_SPRITE_HEIGHT;
            break;

        // Stores BCD representation of the value of the register indexed by the number x in memory locations I, I+1, and I+2
        case 0x33:
        {
            unsigned char hundreds = chip8->registers.V[x] / 100;
            unsigned char tens = chip8->registers.V[x] / 10 % 10;
            unsigned char units = chip8->registers.V[x] % 10;
            setMemory(&chip8->memory, chip8->registers.I, hundreds);
            setMemory(&chip8->memory, chip8->registers.I + 1, tens);
            setMemory(&chip8->memory, chip8->registers.I + 2, units);
            break;
        }

        // Stores register indexed by 0 through register indexed by x in memory starting at location of register I
        case 0x55:
        {
            for (int i = 0; i <= x; i++)
            {
                setMemory(&chip8->memory, chip8->registers.I + i, chip8->registers.V[i]);
            }
            break;
        }

        // Reads register indexed by 0 through register indexed by x in memory starting at location of register I
        case 0x65:
        {
            for (int i = 0; i <= x; i++)
            {
                chip8->registers.V[i] = getMemory(&chip8->memory, chip8->registers.I + i);
            }
            break;
        }
        }
        break;
    }

    // Unexpected opcode value. Prints error to the terminal.
    default:
        printf("Error: wrong opcode value: %X. Can't execute chip8 operation.\n", opcode);
        break;
    }
}

void execChip8(struct Chip8 *chip8, unsigned short opcode)
{
    switch (opcode)
    {
    // Clears the display
    case 0x00E0:
        clearScreen(&chip8->screen);
        break;

    // Returns from a subroutine
    case 0x00EE:
        chip8->registers.PC = popStack(chip8);
        break;

    // Goes to the extended execution function
    default:
        execExtendedChip8(chip8, opcode);
        break;
    }
}