#include <stdio.h>
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
    uint32_t eflags;
    uint8_t* memory;

    /*
    * Program counter
    */
    uint32_t eip;
} emulator;

int main(int argc, char* argv[]) {
    return 0;
}
