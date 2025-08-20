/* THIS FILE HAS TO BE INCLUDED IN EACH TRANSLATION UNIT, OR IT WILL NOT WORK */
#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "diagnostic.export.h"

// This has the purpose of replacing normal memory management functions
// with these ones that track all allocated pointers, this allows to easily
// determine if the program has a memory leak.

#ifdef  MEMORY_WATCH

#define  malloc(SIZE)                   __malloc(SIZE, __FILE__, __LINE__)
#define  calloc(NMEMB, SIZE)            __calloc(NMEMB, SIZE, __FILE__, __LINE__)
#define  free(PTR)                      __free(PTR)
#define  realloc(PTR, SIZE)             __realloc(PTR, SIZE, __FILE__, __LINE__)
#define  reallocarray(PTR, NMEMB, SIZE) __reallocarray(PTR, NMEMB, SIZE, __FILE__, __LINE__)
#define  CHECK_MEMORY                   printf("Memory usage: %"PRId64" bytes\n", (int64_t)__memuse());
#define  STOP_WATCHING                  __end();

#else
#define CHECK_MEMORY
#define STOP_WATCHING
#endif

DIAGNOSTIC_EXPORT void   *__malloc(size_t, const char*, int);
DIAGNOSTIC_EXPORT void   *__calloc(size_t, size_t, const char*, int);
DIAGNOSTIC_EXPORT void    __free(void*);
DIAGNOSTIC_EXPORT void   *__realloc(void*, size_t, const char*, int);
DIAGNOSTIC_EXPORT void   *__reallocarray(void*, size_t, size_t, const char*, int);

// RETURNS the current size of allocated memory
DIAGNOSTIC_EXPORT size_t  __memuse();

// Prints a report for each unfreed block
DIAGNOSTIC_EXPORT void    __end();

#endif