#include <errno.h>
#include <stdio.h>
#include <string.h>

static const char hex[16] = "0123456789abcdef";

/* Same as isalnum(3), but without locale. */
static int
xisalnum(int c)
{
    return (c >= '0' && c <= '9') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z');
}

/* Same as strchr(3). */
static const char *
xstrchr(const char *s, int c)
{
    for (c = (char)c;; s++) {
        if (*s == c) {
            return s;
        } else if (!*s) {
            return 0;
        }
    }
}

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

static int xoptind = 1;
static int xoptopt;
static char *xoptarg;

/* Same as getopt(3) but never prints errors. */
static int
xgetopt(int argc, char * const argv[], const char *optstring)
{
    static int optpos = 1;
    const char *arg;

    arg = xoptind < argc ? argv[xoptind] : 0;
    if (arg && arg[0] == '-' && arg[1] == '-' && !arg[2]) {
        xoptind++;
        return -1;
    } else if (!arg || arg[0] != '-' || !xisalnum(arg[1])) {
        return -1;
    } else {
        const char *opt = xstrchr(optstring, arg[optpos]);
        xoptopt = arg[optpos];
        if (!opt) {
            return '?';
        } else if (opt[1] == ':') {
            if (arg[optpos + 1]) {
                xoptarg = (char *)arg + optpos + 1;
                xoptind++;
                optpos = 1;
                return xoptopt;
            } else if (argv[xoptind + 1]) {
                xoptarg = (char *)argv[xoptind + 1];
                xoptind += 2;
                optpos = 1;
                return xoptopt;
            } else {
                return ':';
            }
        } else {
            if (!arg[++optpos]) {
                xoptind++;
                optpos = 1;
            }
            return xoptopt;
        }
    }
}


static int
usage(FILE *f)
{
    static const char usage[] =
    "usage: hastyhex [-fhlp] [-o FILE]\n"
    "  -h       print this help message\n"
    "  -l       force output line-buffered\n"
    "  -f       force output fully-buffered\n"
    "  -o FILE  output to file instead of standard output\n"
    "  -p       do not output color (\"plain\")\n";
    return fwrite(usage, sizeof(usage)-1, 1, f) && !fflush(f);
}

static const char *
run(int argc, char **argv)
{
    int option;
    FILE *in = stdin;
    FILE *out = stdout;
    const char *outfile = 0;
    enum {MODE_COLOR, MODE_PLAIN} mode = MODE_COLOR;
    enum {BUF_AUTO, BUF_LINE, BUF_FULL} buf_mode = BUF_AUTO;
    static char missing[] = "missing argument: -?";
    static char illegal[] = "illegal option: -?";

#ifdef _WIN32
    { /* Set stdin/stdout to binary mode. */
        int _setmode(int, int);
        _setmode(0, 0x8000);
        _setmode(1, 0x8000);
    }
    { /* Best effort enable ANSI escape processing. */
        __declspec(dllimport) void *__stdcall GetStdHandle(unsigned);
        __declspec(dllimport) int __stdcall GetConsoleMode(void *, unsigned *);
        __declspec(dllimport) int __stdcall SetConsoleMode(void *, unsigned);
        void *handle;
        unsigned mode;
        handle = GetStdHandle(-11); /* STD_OUTPUT_HANDLE */
        if (GetConsoleMode(handle, &mode)) {
            mode |= 0x0004; /* ENABLE_VIRTUAL_TERMINAL_PROCESSING */
            SetConsoleMode(handle, mode); /* ignore errors */
        }
    }
#endif

    while ((option = xgetopt(argc, argv, ":fhlo:p")) != -1) {
        switch (option) {
        case 'f': buf_mode = BUF_FULL;
                  break;
        case 'h': return usage(stdout) ? 0 : "write error";
        case 'l': buf_mode = BUF_LINE;
                  break;
        case 'o': outfile = xoptarg;
                  break;
        case 'p': mode = MODE_PLAIN;
                  break;
        case ':': missing[sizeof(missing)-2] = xoptopt;
                  usage(stderr);
                  return missing;
        case '?': illegal[sizeof(illegal)-2] = xoptopt;
                  usage(stderr);
                  return illegal;
        }
    }

    /* Configure input */
    if (argv[xoptind]) {
        char *path = argv[xoptind];
        if (argv[xoptind+1]) {
            return "too many arguments";
        }
        if (path[0] != '-' || path[1] != 0) {
            in = fopen(path, "rb");
            if (!in) {
                return path;
            }
        }
    }

    /* Configure output */
    if (outfile) {
        out = fopen(outfile, "wb");
        if (!out) {
            return outfile;
        }
    }
    switch (buf_mode) {
    static char buf[1L << 18];
    case BUF_AUTO: break;
    case BUF_LINE: setvbuf(out, buf, _IOLBF, sizeof(buf)); break;
    case BUF_FULL: setvbuf(out, buf, _IOFBF, sizeof(buf)); break;
    }

    /* Process input using user-selected mode */
    switch (mode) {
    case MODE_COLOR: process_color(in, out); break;
    case MODE_PLAIN: process_plain(in, out); break;
    }

    /* Check for errors before exiting */
    if (ferror(in)) {
        return "input error";
    }
    if (fflush(out) || ferror(out)) {
        return "output error";
    }
    return 0;
}

int
main(int argc, char **argv)
{
    const char *err = run(argc, argv);
    if (err) {
        fputs("hastyhex: ", stderr);
        if (errno) {
            fputs(strerror(errno), stderr);
            fputs(": ", stderr);
        }
        fputs(err, stderr);
        fputs("\n", stderr);
        return 1;
    }
    return 0;
}
