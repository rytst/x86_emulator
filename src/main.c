#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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


typedef void (*instruction_func_t)(emulator* emu);
instruction_func_t instructions[256];


static emulator* init_emulator(size_t mem_size, uint32_t eip, uint32_t esp);
static void      destroy_emulator(emulator* emu);
static void      mov_r32_imm32(emulator* emu);
static int8_t    get_sign_code8(emulator* emu, int index);
static uint8_t   get_code8(emulator* emu, int index);
static uint32_t  get_code32(emulator* emu, int index);
static void      short_jump(emulator* emu);
static void      init_instructions(void);
static void      dump_registers(emulator* emu);




int main(int argc, char* argv[]) {
    FILE* fp;
    emulator* emu;

    /*
     * Command line usage
     */
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

    init_instructions();

    for ( ;emu->eip < MEMORY_SIZE; ) {
        uint8_t code = get_code8(emu, 0);

        if (instructions[code] == NULL) {
            fprintf(stderr, "No instructions for %x\n", code);
            break;
        }

        instructions[code](emu);

        if (emu->eip == 0x00) {
            printf("Program finished.\n");
            break;
        }
    }

    dump_registers(emu);
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


static void mov_r32_imm32(emulator* emu) {
    uint8_t  reg   = get_code8(emu, 0) - 0xB8;
    uint32_t imm32 = get_code32(emu, 1);

    emu->registers[reg] = imm32;

    /*
     * Next program counter
     */
    emu->eip += 5;
}


static int8_t get_sign_code8(emulator* emu, int index) {
    return (int8_t)emu->memory[emu->eip + index];
}


static uint8_t get_code8(emulator* emu, int index) {
    return emu->memory[emu->eip + index];
}

static uint32_t get_code32(emulator* emu, int index) {
    uint32_t ret = 0;

    /*
     * Get value with little endian byte order
     */
    for (int i=0; i < 4; i++) {
        ret |= get_code8(emu, index+i) << (i * 8);
    }

    return ret;
}


static void short_jump(emulator* emu) {
    int8_t diff = get_sign_code8(emu, 1);
    emu->eip += 2 + diff;
}


static void init_instructions(void) {
    memset(instructions, 0, sizeof(instructions));

    for (int i=0; i < REGISTERS_COUNT; i++) {
        instructions[0xB8 + i] = &mov_r32_imm32;
    }
    instructions[0xEB] = &short_jump;
}


static void dump_registers(emulator* emu) {
    for (int i=0; i < REGISTERS_COUNT; i++) {
        printf("%s = %08x\n", registers_name[i], emu->registers[i]);
    }

    printf("EIP = %08x\n", emu->eip);
}
