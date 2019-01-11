/* Hexdump in color
 *
 * Only supports ANSI escapes, going from standard input to standard
 * output. It doesn't care whether or not the output is a TTY.
 *
 * This is free and unencumbered software released into the public domain.
 */
#include <stdio.h>

#define PROGRAM_NAME "hexcolor"

#define CN 0x37 /* NULL    */
#define CS 0x92 /* SPACE   */
#define CP 0x96 /* PRINT   */
#define CC 0x95 /* CONTROL */
#define CH 0x93 /* HIGH    */

static int
color(int b)
{
    static const unsigned char table[] = {
        CN, CC, CC, CC, CC, CC, CC, CC, CC, CC, CS, CS, CS, CS, CC, CC,
        CC, CC, CC, CC, CC, CC, CC, CC, CC, CC, CC, CC, CC, CC, CC, CC,
        CS, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP,
        CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP,
        CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP,
        CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP,
        CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP,
        CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CP, CC,
        CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH,
        CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH,
        CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH,
        CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH,
        CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH,
        CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH,
        CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH,
        CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH, CH
    };
    return table[b];
}

int
main(void)
{
    int i;
    char line[256];
    char *p, *raw, *ascii;
    unsigned long off = 0;
    static const char hex[16] = "0123456789abcdef";

    /* 32-bit offset counter */
    p = line;
    for (i = 0; i < 8; i++)
        *p++ = '0';
    *p++ = ' ';
    *p++ = ' ';

    /* Input in hexidecimal */
    raw = p;
    for (i = 0; i < 16; i++) {
        *p++ = 0x1b;
        *p++ = '[';
        *p++ = 'X';
        *p++ = 'X';
        *p++ = 'm';
        *p++ = '0';
        *p++ = '0';
        *p++ = ' ';
    }
    *p++ = ' ';

    /* Input in ASCII */
    ascii = p;
    for (i = 0; i < 16; i++) {
        *p++ = 0x1b;
        *p++ = '[';
        *p++ = 'X';
        *p++ = 'X';
        *p++ = 'm';
        *p++ = '.';
    }
    *p++ = 0x1b;
    *p++ = '[';
    *p++ = '0';
    *p++ = 'm';
    *p++ = '\n';
    *p   = 0;

    for (;;) {
        int n;
        unsigned char input[16];

        n = fread(input, 1, 16, stdin);
        if (!n)
            break;

        /* Write the offset */
        for (i = 0; i < 8; i++)
            line[i] = hex[(off >> (28 - i * 4)) & 15];

        /* Write in all the colors */
        for (i = 0; i < 16; i++) {
            int v = input[i];
            int c = color(v);
            raw[i * 8 + 2] = hex[c >> 4];
            raw[i * 8 + 3] = hex[c & 15];
            raw[i * 8 + 5] = hex[v >> 4];
            raw[i * 8 + 6] = hex[v & 15];
            ascii[i * 6 + 2] = hex[c >> 4];
            ascii[i * 6 + 3] = hex[c & 15];
            ascii[i * 6 + 5] = v == ' ' || c == CP ? v : '.';
        }

        /* Erase any trailing bytes */
        for (i = n; i < 16; i++) {
            raw[i * 8 + 5] = ' ';
            raw[i * 8 + 6] = ' ';
            ascii[i * 6 + 5] = ' ';
        }

        if (!fwrite(line, p - line, 1, stdout))
            break;
        off += 16;
    }

    if (ferror(stdin)) {
        fputs(PROGRAM_NAME ": input error\n", stderr);
        return 1;
    }
    if (fflush(stdout) || ferror(stdout)) {
        fputs(PROGRAM_NAME ": output error\n", stderr);
        return 1;
    }
    return 0;
}
