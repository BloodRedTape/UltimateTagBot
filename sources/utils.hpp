#pragma once

#include <string>

std::string ToLowerCase(std::string str);

int CountChars(char ch, const char *string);

inline int CountChars(char ch, const std::string &string){ return CountChars(ch, string.c_str()); }