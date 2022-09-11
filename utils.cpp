#include "utils.hpp"
#include <boost/locale/conversion.hpp>
#include <boost/locale/generator.hpp>

std::string ToLowerCase(std::string str){
    return boost::locale::to_lower(str, boost::locale::generator()(""));
}

int CountChars(char ch, const std::string &string){
    int count = 0;
    for(char e: string)
        count += e == ch;
    return count;
}