#include "strings_extension.h"
#include "strings.h"
#include "printf.h"
#include "malloc.h"
#include "ctype.h"

int strcasecmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        char c1 = *s1++;
        char c2 = *s2++;
        if ('A' <= c1 && c1 <= 'Z')
            c1 += 'a' - 'A';
        if ('A' <= c2 && c2 <= 'Z')
            c2 += 'a' - 'A';
        if (c1 != c2)
            return (unsigned char)c1 - (unsigned char)c2;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

int strncasecmp(const char *s1, const char *s2, unsigned long n) {
    while (n > 0) {
        char c1 = toupper((unsigned char)*s1++);
        char c2 = toupper((unsigned char)*s2++);

        if (c1 != c2)
            return c1 - c2;

        if (c1 == '\0')
            return 0;

        n--;
    }
    return 0;
}

// #undef strncpy
char *strncpy(char *dest, const char *src, size_t n) {
    int i = 0;
    while (i < n && src[i] != '\0') {
        dest[i] = src[i];
        i+=1;
    }
    while (i < n) {
        dest[i] = '\0';
        i+=1;
    }
    return dest;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    size_t i = 0;
    while (i < n) {
        if (s1[i] < s2[i]) return -1;
        if (s1[i] > s2[i]) return 1;
        if (s1[i] == '\0' && s2[i] == '\0') break;
        i++;
    }
    return 0;
}

char *strdup(const char *s) {
    if (s == NULL)
        return NULL;
    size_t size = strlen(s);

    char *duped = (char *)malloc(size + 1);
    if (duped == NULL)
        return NULL;

    #undef memcpy
    memcpy(duped, s, size);
    return duped;
}

char *strchr(const char *str, int c) {
    const char *ret = NULL;
    char target_char = (char)c;

    while (*str != '\0') {
        if (*str == target_char) {
            ret = str;
            break;
        }
        str++;
    }

    if (target_char == '\0')
        ret = str;
    return (char *)ret;
}

char *strrchr(const char *str, int c) {
    const char *ret = NULL;
    char target_char = (char)c;

    while (*str != '\0') {
        if (*str == target_char)
            ret = str;
        str++;
    }

    if (target_char == '\0')
        ret = str;
    return (char *)ret;
}

char *strstr(const char *s1, const char *s2) {
    size_t n = strlen(s2);
    while (*s1) {
        if (!strcmp(s1++, s2))
            return (char *)(s1 - 1);
    }
    return 0;
}

int puts(const char *s) {
    int ret = printf("%s\n", s);
    return ret;
}

int putchar(int ch) {
    printf("%c\n", ch);
    return ch;
}

#undef memmove
void *memmove(void *dest, const void *src, size_t n) {
    unsigned char *temp = malloc(n);
    if (temp == NULL)
        return NULL;
    memcpy(temp, src, n);
    memcpy(dest, temp, n);
    free(temp);
    return dest;
}