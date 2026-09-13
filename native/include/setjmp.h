#ifndef MELEE_NATIVE_SETJMP_H
#define MELEE_NATIVE_SETJMP_H
#include_next <setjmp.h>
// Upstream now uses the standard setjmp/longjmp API. Keep the full host
// context and do not redeclare libc's jmp_buf or its internal spellings.
#endif
