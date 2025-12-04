/*
 * stack-L1.c -- 32-bit vulnerable Set-UID style program for Level 1
 * Prints buffer/EBP/RET locations at runtime (no gdb needed).
 * If PROBE is set, it only prints addresses and exits; otherwise it
 * copies the content of "badfile" into a fixed-size stack buffer (strcpy overflow).
 *
 * Build (32-bit, execstack, no SSP, no PIE):
 * gcc -m32 -O0 -fno-omit-frame-pointer -z execstack -fno-stack-protector \
 *     -no-pie -o stack-L1 stack-L1.c
 * Optional Set-UID demo (requires root):
 * sudo chown root:root stack-L1 && sudo chmod 4755 stack-L1
 * Recommended for Level 1 shell spawning:
 * sudo ln -sf /bin/zsh /bin/sh
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#ifndef BUF_SIZE
#define BUF_SIZE 100
#endif

/* Use builtin to read the current function's frame address (EBP). */
static inline void *get_ebp(void) {
    return __builtin_frame_address(0);
}

static void bof(const char *src)
{
    char buffer[BUF_SIZE];

    /* Compute and print buffer address, current EBP, saved RET location, and offset */
    void *ebp_ptr = get_ebp();           /* current EBP */
    uintptr_t buf = (uintptr_t)buffer;
    uintptr_t ebp = (uintptr_t)ebp_ptr;
    uintptr_t retloc = ebp + 4;          /* saved return address */
    unsigned offset = (unsigned)(retloc - buf); /* from buffer start to saved RET */

    printf("[ADDR] BUF=0x%08lx EBP=0x%08lx RETLOC=0x%08lx OFFSET=%u\n",
           (unsigned long)buf, (unsigned long)ebp, (unsigned long)retloc, offset);
    fflush(stdout);

    /* In PROBE mode we only print addresses and return early. */
    if (getenv("PROBE")) return;

    /* Vulnerable copy: overflows 'buffer' when fed with a 517-byte "badfile". */
    strcpy(buffer, src);

    /* If exploitation fails, the function returns normally and prints this: */
    printf("Returned Properly\n");
}

int main(int argc, char *argv[])
{
    /* In probe mode, call bof() with a dummy string; do NOT require badfile. */
    if (getenv("PROBE")) {
        bof("DUMMY");
        return 0;
    }

    /* Otherwise read attacker-controlled input from "badfile". */
    char str[517];
    FILE *f = fopen("badfile", "rb");
    if (!f) { perror("fopen badfile"); return 1; }
    int n = (int)fread(str, 1, sizeof(str) - 1, f);
    fclose(f);
    str[n] = '\0';

    bof(str);
    return 0;
}


