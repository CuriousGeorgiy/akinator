#include "String.hpp"

#include <cctype>
#include <cstring>

void toLowerStr(char *str)
{
    while (*str) {
        tolower(*str++);
    }
}

char *readLineRemoveNewline(std::FILE *file, char *buffer, size_t maxLength)
{
    std::fgets(buffer, maxLength, stdin);
    size_t answerLength = std::strlen(buffer);
    auto line = new char[answerLength + 1];
    std::strcpy(line, buffer);
    if (line[answerLength - 1] == '\n') line[answerLength - 1] = '\0';

    return line;
}

char *skipWhitespaces(char *str)
{
    while (*str && isspace(*str)) ++str;

    return str;
}
