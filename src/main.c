#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

enum register {
    EAX,
    ECX,
    EDX,
    EBX,
    ESP,
    EBP,
    ESI,
    EDI,
    REGISTERS_COUNT
};

char* registers_name[] = {
    "EAX",
    "ECX",
    "EDX",
    "EBX",
    "ESP",
    "EBP",
    "ESI",
    "EDI"
};

typedef struct {
    uint32_t registers[REGISTERS_COUNT];

    /*
     * EFLAGS register
     */
    uint32_t eflags;

    uint8_t* memory;

    /*
    * Program counter (Instruction Pointer)
    */
    uint32_t eip;
} emulator;

static emulator* init_emu(size_t mem_size, uint32_t eip, uint32_t esp);
static void      destroy_emu(emulator* emu);

int main(int argc, char* argv[]) {
    return 0;
}

static emulator* init_emu(size_t mem_size, uint32_t eip, uint32_t esp) {

    /*
     * Generate emulator
     */
    emulator* emu = (emulator*)malloc(sizeof(emulator));


    /*
     * Initialize emulator
     */
    emu->memory        = (uint8_t*)calloc(sizeof(uint8_t), mem_size);
    emu->eip           = eip;
    emu->register[ESP] = esp;

    return emu;
}
