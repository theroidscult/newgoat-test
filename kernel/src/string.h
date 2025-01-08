#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);

uint64_t strlen(const char *s);
bool ends_with(const char *s, const char *suffix);

#endif // __STRING_H__