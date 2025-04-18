#define INCBIN(identifier, filename)                \
    asm(".pushsection .rodata\n"                    \
        "\t.local " #identifier "\n"                \
        "\t.type " #identifier ", @object\n"        \
        "\t.balign 8\n"                             \
        #identifier ":\n"                           \
        "\t.incbin \"" filename "\"\n\n"            \
                                                    \
        "\t.local " #identifier "_end\n"            \
        "\t.type " #identifier "_end, @object\n"    \
        #identifier "_end:\n"                       \
                                                    \
        "\t.balign 8\n"                             \
        "\t.popsection\n");                         \
    extern u8 identifier[];                         \
    extern u8 identifier##_end[];