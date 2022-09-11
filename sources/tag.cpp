#include "tag.hpp"
#include "utils.hpp"

static bool IsValidCharacters(const std::string &tag){
    for(char ch: tag){
        if(!(ch >= 'a' && ch <= 'z') 
        && !(ch >= 'A' && ch <= 'Z')
        && !(ch >= '0' && ch <= '9')
        && !(ch == '_'))
            return false;
    }
    return true;
}

Tag::Tag(std::string tag){
    if(!tag.size() || tag[0] != '@')
        return;

    assign(ToLowerCase(tag.substr(1, tag.size() - 1)));
}

bool Tag::IsValid()const{
    if(!(size() >= MinLength && size() <= MaxLength) || !IsValidCharacters(*this))
        return false;
    return true; 
}