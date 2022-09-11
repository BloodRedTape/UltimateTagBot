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
    std::string keytag = tag.substr(1, tag.size() - 1);
    
    if(!(keytag.size() >= MinLength && keytag.size() <= MaxLength) || !IsValidCharacters(keytag))
        return;
    
    assign(ToLowerCase(keytag));
}

bool Tag::IsValid()const{
    return size(); 
}