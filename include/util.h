#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <time.h>

#define true 1
#define false 0

#define SIZEOF_ARRAY(a) (sizeof(a)/sizeof(a[0]))

#ifndef __cplusplus
typedef char bool;
#endif

clock_t __start__, __end__;

#define START_CLOCK                         \
    if ((__start__ = clock()) == -1) {      \
        printf("Clock failed to start\n");  \
        exit(1);                            \
    }

#define STOP_CLOCK                          \
    if ((__end__ = clock()) == -1) {        \
        printf("Clock failed to stop\n");   \
        exit(1);                            \
    }

#define TIME_ELAPSED (((double)__end__-__start__) / CLOCKS_PER_SEC)

#define strequ(a, b) (strcmp(a, b) == 0)

#ifdef __cplusplus
}
#endif
