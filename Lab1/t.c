#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int *FP; // Save C()'s frame pointer

/* ---- Inline getebp(): 32-bit x86 only ----
 * Option A: GCC builtin (also requires -fno-omit-frame-pointer)
 */
// static inline int *getebp(void) {
//     return (int *)__builtin_frame_address(0);
// }

/* Option B: inline assembly (matches your ts.s exactly) */
static inline int *getebp(void) {
#if defined(__i386__)
    int *ebp;
    __asm__ __volatile__("movl %%ebp, %0" : "=r"(ebp));
    return ebp;
#else
# error "getebp() inline asm is for 32-bit x86 (i386) only."
#endif
}

int A(int x, int y);
int B(int x, int y);
int C(int x, int y);

static void print_frame_chain(int *fp) {
    printf("\n[Frame Chain]\n");
    int *cur = fp;
    for (int depth = 0; cur && depth < 20; ++depth) {
        printf(" #%02d FP=0x%08x prevFP=0x%08x\n",
               depth, (unsigned int)cur, (unsigned int)(*(unsigned int *)cur));
        cur = (int *)(*(unsigned int *)cur);
    }
    printf(" ... -> NULL (or limit)\n");
}

static void dump_stack_from(int *p, int max_words) {
    printf("\n[Stack Dump] from p=%p, %d words (4B each)\n", (void*)p, max_words);
    for (int k = 0; k < max_words; ++k) {
        int *addr = p + k;
        unsigned int val = *(unsigned int *)addr;
        printf(" p[%03d] @ %p : 0x%08x\n", k, (void*)addr, val);
    }
}

int main(int argc, char *argv[], char *env[]) {
    int a, b, c;
    printf("enter main\n");

    printf("&argc=%08x argv=%08x env=%08x\n",
           (unsigned int)&argc, (unsigned int)argv, (unsigned int)env);

    printf("&a=%08x &b=%08x &c=%08x\n",
           (unsigned int)&a, (unsigned int)&b, (unsigned int)&c);

    // (1) Print argc and argv[] entries (values + addresses)
    printf("\n[ARGV]\n");
    printf("argc=%d\n", argc);
    for (int i = 0; i < argc; ++i) {
        printf(" argv[%d]=\"%s\" &argv[%d]=%08x argv[%d]_ptr=%08x\n",
               i, argv[i],
               i, (unsigned int)&argv[i],
               i, (unsigned int)argv[i]);
    }

    a = 1; b = 2; c = 3;
    A(a, b);
    printf("exit main\n");
    return 0;
}

int A(int x, int y) {
    int d, e, f;
    printf("\nenter A\n");

    printf("[A locals ] &d=%08x &e=%08x &f=%08x\n",
           (unsigned int)&d, (unsigned int)&e, (unsigned int)&f);
    printf("[A params ] &x=%08x &y=%08x\n",
           (unsigned int)&x, (unsigned int)&y);

    d = 4; e = 5; f = 6;
    B(d, e);
    printf("exit A\n");
    return 0;
}

int B(int x, int y) {
    int g, h, i;
    printf("\nenter B\n");

    printf("[B locals ] &g=%08x &h=%08x &i=%08x\n",
           (unsigned int)&g, (unsigned int)&h, (unsigned int)&i);
    printf("[B params ] &x=%08x &y=%08x\n",
           (unsigned int)&x, (unsigned int)&y);

    g = 7; h = 8; i = 9;
    C(g, h);
    printf("exit B\n");
    return 0;
}

int C(int x, int y) {
    int u, v, w, i, *p;

    printf("\nenter C\n");
    printf("[C locals ] &u=%08x &v=%08x &w=%08x &i=%08x &p=%08x\n",
           (unsigned int)&u, (unsigned int)&v, (unsigned int)&w,
           (unsigned int)&i, (unsigned int)&p);
    printf("[C params ] &x=%08x &y=%08x\n",
           (unsigned int)&x, (unsigned int)&y);

    u = 10; v = 11; w = 12; i = 13;

    FP = getebp(); // C()'s EBP

    unsigned int saved_ret = *((unsigned int *)FP + 1); // *(EBP+4)
    printf("\n[FP/Saved RET]\n");
    printf(" FP(C.EBP) = 0x%08x\n", (unsigned int)FP);
    printf(" RET(EBP+4)= 0x%08x\n", saved_ret);

    print_frame_chain(FP);

    p = &u;
    dump_stack_from(p, 128);

    printf("exit C\n");
    return 0;
}

