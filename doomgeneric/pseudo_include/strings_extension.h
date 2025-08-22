#include <stddef.h>
#pragma once

// Replacement for standard string.h lib
int strcasecmp(const char *s1, const char *s2);

int strncasecmp(const char *s1, const char *s2, unsigned long n);

char *strncpy(char *dest, const char *src, size_t n);

int strncmp(const char *s1, const char *s2, size_t n);

char *strdup(const char *s);

char *strchr(const char *str, int c);

char *strrchr(const char *str, int c);

char *strstr(const char *s1, const char *s2);

int puts(const char *s);

int putchar(int ch);

void *memmove(void *dest, const void *src, size_t n);