#ifndef _PRINTING_HPP_
#define _PRINTING_HPP_

#include "../h/syscall_c.hpp"

typedef unsigned long uint64;

extern "C" uint64 copy_and_swap(uint64 &lock, uint64 expected, uint64 desired);

void printS(char const *string);

char* getS(char *buf, int max);

int stringToI(const char *s);

void printI(int xx, int base=10, int sgn=0);

#endif // _PRINTING_HPP_

