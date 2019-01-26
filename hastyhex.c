#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "getopt.h"

#define PROGRAM_NAME "hastyhex"

static const char hex[16] = "0123456789abcdef";

static int
color(int b)
{
    #define CN 0x37 /* null    */
    #define CS 0x92 /* space   */
    #define CP 0x96 /* print   */
    #define CC 0x95 /* control */
    #define CH 0x93 /* high    */
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

static int
display(int b)
{
    static const char table[] = {
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x20,
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b,
        0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
        0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41,
        0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c,
        0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
        0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62,
        0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d,
        0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
        0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
        0x2e, 0x2e, 0x2e,
    };
    return table[b];
}

static void
process_color(FILE *in, FILE *out)
{
    int i, n;
    unsigned long offset = 0;
    unsigned char input[16] = {0};
    char template[] =
        "00000000  "
        "\33[XXm## \33[XXm## \33[XXm## \33[XXm## "
        "\33[XXm## \33[XXm## \33[XXm## \33[XXm##  "
        "\33[XXm## \33[XXm## \33[XXm## \33[XXm## "
        "\33[XXm## \33[XXm## \33[XXm## \33[XXm##  "
        "\33[XXm.\33[XXm.\33[XXm.\33[XXm.\33[XXm.\33[XXm.\33[XXm.\33[XXm."
        "\33[XXm.\33[XXm.\33[XXm.\33[XXm.\33[XXm.\33[XXm.\33[XXm.\33[XXm."
        "\33[0m\n";
    static const int slots[] = {
        /* ANSI-color, hex, ANSI-color, ASCII */
         12,  15, 142, 145,
         20,  23, 148, 151,
         28,  31, 154, 157,
         36,  39, 160, 163,
         44,  47, 166, 169,
         52,  55, 172, 175,
         60,  63, 178, 181,
         68,  71, 184, 187,
         77,  80, 190, 193,
         85,  88, 196, 199,
         93,  96, 202, 205,
        101, 104, 208, 211,
        109, 112, 214, 217,
        117, 120, 220, 223,
        125, 128, 226, 229,
        133, 136, 232, 235
    };

    do {
        n = fread(input, 1, 16, in);

        /* Write the offset */
        for (i = 0; i < 8; i++)
            template[i] = hex[(offset >> (28 - i * 4)) & 15];

        /* Fill out the template */
        for (i = 0; i < 16; i++) {
            /* Use a fixed loop count instead of "n" to encourage loop
             * unrolling by the compiler. Empty bytes will be erased
             * later.
             */
            int v = input[i];
            int c = color(v);
            template[slots[i * 4 + 0] + 0] = hex[c >> 4];
            template[slots[i * 4 + 0] + 1] = hex[c & 15];
            template[slots[i * 4 + 1] + 0] = hex[v >> 4];
            template[slots[i * 4 + 1] + 1] = hex[v & 15];
            template[slots[i * 4 + 2] + 0] = hex[c >> 4];
            template[slots[i * 4 + 2] + 1] = hex[c & 15];
            template[slots[i * 4 + 3] + 0] = display(v);
        }

        /* Erase any trailing bytes */
        for (i = n; i < 16; i++) {
            /* This loop is only used once: the last line of output. The
             * branch predictor will quickly learn that it's never taken.
             */
            template[slots[i * 4 + 0] + 0] = '0';
            template[slots[i * 4 + 0] + 1] = '0';
            template[slots[i * 4 + 1] + 0] = ' ';
            template[slots[i * 4 + 1] + 1] = ' ';
            template[slots[i * 4 + 2] + 0] = '0';
            template[slots[i * 4 + 2] + 1] = '0';
            template[slots[i * 4 + 3] + 0] = ' ';
        }

        if (!fwrite(template, sizeof(template) - 1, 1, out))
            break; /* Output error */
        offset += 16;
    } while (n == 16);
}

static void
process_plain(FILE *in, FILE *out)
{
    int i, n;
    unsigned long offset = 0;
    unsigned char input[16] = {0};
    char template[] =
        "00000000  ## ## ## ## ## ## ## ##  ## ## ## ## ## ## ## ##  "
        "................\n";
    static const int slots[] = {
        10, 60, 13, 61, 16, 62, 19, 63, 22, 64, 25, 65, 28, 66, 31, 67,
        35, 68, 38, 69, 41, 70, 44, 71, 47, 72, 50, 73, 53, 74, 56, 75
    };

    do {
        n = fread(input, 1, 16, in);

        /* Write the offset */
        for (i = 0; i < 8; i++)
            template[i] = hex[(offset >> (28 - i * 4)) & 15];

        /* Fill out the template */
        for (i = 0; i < 16; i++) {
            int v = input[i];
            template[slots[i * 2 + 0] + 0] = hex[v >> 4];
            template[slots[i * 2 + 0] + 1] = hex[v & 15];
            template[slots[i * 2 + 1] + 0] = display(v);
        }

        /* Erase any trailing bytes */
        for (i = n; i < 16; i++) {
            template[slots[i * 2 + 0] + 0] = ' ';
            template[slots[i * 2 + 0] + 1] = ' ';
            template[slots[i * 2 + 1] + 0] = ' ';
        }

        if (!fwrite(template, sizeof(template) - 1, 1, out))
            break;
        offset += 16;
    } while (n == 16);
}

static void
usage(FILE *f)
{
    fprintf(f, "usage: " PROGRAM_NAME " [-fhlp] [-o FILE]\n");
    fprintf(f, "  -h       print this help message\n");
    fprintf(f, "  -l       force output line-buffered\n");
    fprintf(f, "  -f       force output fully-buffered\n");
    fprintf(f, "  -o FILE  output to file instead of standard output\n");
    fprintf(f, "  -p       do not output color (\"plain\")\n");
}

int
main(int argc, char *argv[])
{
    int option;
    FILE *in = stdin;
    FILE *out = stdout;
    const char *outfile = 0;
    enum {MODE_COLOR, MODE_PLAIN} mode = MODE_COLOR;
    enum {BUF_AUTO, BUF_LINE, BUF_FULL} buf_mode = BUF_AUTO;

#ifdef _WIN32
    int _setmode(int, int);
    _setmode(_fileno(stdout), 0x8000);
    _setmode(_fileno(stdin), 0x8000);
#endif

    while ((option = getopt(argc, argv, "fhlo:p")) != -1) {
        switch (option) {
            case 'f':
                buf_mode = BUF_FULL;
                break;
            case 'h':
                usage(stdout);
                return 0;
            case 'l':
                buf_mode = BUF_LINE;
                break;
            case 'o':
                outfile = optarg;
                break;
            case 'p':
                mode = MODE_PLAIN;
                break;
            default:
                return 1;
        }
    }

    /* Configure input */
    if (argv[optind]) {
        if (argv[optind + 1]) {
            fprintf(stderr, PROGRAM_NAME ": too many arguments\n");
            return 1;
        }
        if (strcmp(argv[optind], "-")) {
            in = fopen(argv[optind], "rb");
            if (!in) {
                fprintf(stderr, PROGRAM_NAME ": %s -- %s\n",
                        strerror(errno), argv[optind]);
                return 1;
            }
        }
    }

    /* Configure output */
    if (outfile) {
        out = fopen(outfile, "wb");
        if (!out) {
            fprintf(stderr, PROGRAM_NAME ": %s -- %s\n",
                    strerror(errno), outfile);
            return 1;
        }
    }
    switch (buf_mode) {
        static char buf[1L << 18];
        case BUF_AUTO:
            break;
        case BUF_LINE:
            setvbuf(out, buf, _IOLBF, sizeof(buf));
            break;
        case BUF_FULL:
            setvbuf(out, buf, _IOFBF, sizeof(buf));
            break;
    }

    /* Process input using user-selected mode */
    switch (mode) {
        case MODE_COLOR:
            process_color(in, out);
            break;
        case MODE_PLAIN:
            process_plain(in, out);
            break;
    }

    /* Check for errors before exiting */
    if (ferror(in)) {
        fputs(PROGRAM_NAME ": input error\n", stderr);
        return 1;
    }
    if (fflush(out) || ferror(out)) {
        fputs(PROGRAM_NAME ": output error\n", stderr);
        return 1;
    }
    return 0;
}
