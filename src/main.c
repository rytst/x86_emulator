#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MEMORY_SIZE 512 * 512

enum x86_register {
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


/*
 * Data structure of emulator
 */
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

static emulator* init_emulator(size_t mem_size, uint32_t eip, uint32_t esp);
static void      destroy_emulator(emulator* emu);




int main(int argc, char* argv[]) {
    FILE* fp;
    emulator* emu;

    if (argc != 2) {
        fprintf(stderr, "usage: x86_emulator filename\n");
        exit(1);
    }

    emu = init_emulator(MEMORY_SIZE, 0x0000, 0x7c00);

    char* filename = argv[1];
    fp = fopen(filename, "rb");

    /*
     * Error handling for opening file
     */
    if (fp == NULL) {
        fprintf(stderr, "Can not open %s\n", filename);
        exit(1);
    }

    /*
     * Load data into memory
     */
    fread(emu->memory, 1, 0x200, fp);
    fclose(fp);

    destroy_emulator(emu);

    return 0;
}


/*
 * Generate and initialize emulator
 */
static emulator* init_emulator(size_t mem_size, uint32_t eip, uint32_t esp) {

    /*
     * Generate emulator
     */
    emulator* emu = (emulator*)malloc(sizeof(emulator));


    /*
     * Initialize emulator
     */
    emu->memory         = (uint8_t*)calloc(sizeof(uint8_t), mem_size);
    emu->eip            = eip;
    emu->registers[ESP] = esp;

    if (emu->memory == NULL) {
        fprintf(stderr, "Memory not allocated");
        exit(1);
    }

    return emu;
}

static void destroy_emulator(emulator* emu) {
    free(emu->memory);
    free(emu);
}
