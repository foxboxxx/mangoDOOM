#include "ctype.h"

int isalpha(int c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return 1;
    else return 0;
}

int islower(int c) {
    if (c >= 'a' && c <= 'z') return 1;
    else return 0; 
}

int isspace(int c) {
    unsigned char u = (unsigned char)c; 
    if (u == ' ' || u == '\t' ||  u == '\n' || u == '\v' || u == '\f' || u == '\r')  return 1; 
    else return 0; 
}

int toupper(int ch) {
    if (ch >= 'a' && ch <= 'z') return ch - 32;
    else return ch;
}

int tolower(int ch) {
    if (ch >= 'A' && ch <= 'Z') return ch + 32;
    else return ch;
}