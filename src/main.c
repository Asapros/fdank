#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <elf.h>
#include <stdbool.h>

#define BASE_VADDR 0x400000

#define OPCODE_MOVE 0xB8
#define OPCODE_SYSCALL_1 0x0F
#define OPCODE_SYSCALL_2 0x05

// REX prefix
#define REX_BASE 0x40
#define REX_W    0x08
#define REX_R    0x04
#define REX_X    0x02
#define REX_B    0x01

// Register numbers
#define OPCODE_REGISTER_AX 0
#define OPCODE_REGISTER_CX 1
#define OPCODE_REGISTER_DX 2
#define OPCODE_REGISTER_BX 3
#define OPCODE_REGISTER_SP 4
#define OPCODE_REGISTER_BP 5
#define OPCODE_REGISTER_SI 6
#define OPCODE_REGISTER_DI 7

void readlines() {
    Lexer lexer;
    lexer_init(&lexer, stdin);
    Token token;
    Parser parser;
    parser_init(&parser, lexer);
    while (true) {
        parser_next_instruction(&parser);
        if (parser.next_token.type == END) {
            break;
        }
    };
}

int main() {
    unsigned char code[] = {
        0xB8, 0x3C, 0x00, 0x00, 0x00, // mov eax, 60 (sys_exit)
        0xBF, 0x45, 0x00, 0x00, 0x00, // mov edi, 69 (exit status)
        0x0F, 0x05                    // syscall
    };
    Elf64_Ehdr ehdr = {
        .e_ident = {
            0x7f, 'E', 'L', 'F',
            ELFCLASS64,
            ELFDATA2LSB,
            EV_CURRENT,
            ELFOSABI_SYSV
        },
        .e_type      = ET_EXEC,
        .e_machine   = EM_X86_64,
        .e_version   = EV_CURRENT,
        .e_entry     = BASE_VADDR + sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr),
        .e_phoff     = sizeof(Elf64_Ehdr),
        .e_shoff     = 0,
        .e_flags     = 0,
        .e_ehsize    = sizeof(Elf64_Ehdr),
        .e_phentsize = sizeof(Elf64_Phdr),
        .e_phnum     = 1,
        .e_shentsize = 0,
        .e_shnum     = 0,
        .e_shstrndx  = SHN_UNDEF
    };

    Elf64_Phdr phdr = {
        .p_type   = PT_LOAD,
        .p_flags  = PF_R | PF_X,
        .p_offset = 0,
        .p_vaddr  = BASE_VADDR,
        .p_paddr  = 0,
        .p_filesz = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) + sizeof(code),
        .p_memsz  = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) + sizeof(code),
        .p_align  = 0x1000
    };
    readlines();

    FILE *file = fopen("a.out", "wb");

    fwrite(&ehdr, sizeof(ehdr), 1, file);
    fwrite(&phdr, sizeof(phdr), 1, file);
    fwrite(code, sizeof(code), 1, file);

    fclose(file);


    return 0;
}
