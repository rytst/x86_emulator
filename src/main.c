#include <stdio.h>
#include <stdint.h>

#define REGISTERS_COUNT 1000000;

typedef struct {
    uint32_t registers[REGISTERS_COUNT];
    uint32_t eflags;
    uint8_t* memory;
    uint32_t eip;
} emulator;

int main(void) {
    return 0;
}
