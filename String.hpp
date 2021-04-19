#ifndef STRING_HPP
#define STRING_HPP

#include <cstdio>

void toLowerStr(char *str);
char *readLineRemoveNewline(std::FILE *file, char *buffer, size_t maxLength);
char *skipWhitespaces(char *str);

#endif /* STRING_HPP */
