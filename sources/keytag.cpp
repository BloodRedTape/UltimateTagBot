#include "keytag.hpp"
#include "utils.hpp"

static bool IsValidCharacters(const std::string &tag){
    for(char ch: tag){
        if(!Keytag::IsKeytagChar(ch))
            return false;
    }
    return true;
}

Keytag::Keytag(std::string keytag){
    assign(ToLowerCase(keytag));
}

bool Keytag::IsValid()const{
    return size() && IsValidCharacters(*this);
}
bool Keytag::IsKeytagChar(char ch){
    return (ch >= 'a' && ch <= 'z') 
        || (ch >= 'A' && ch <= 'Z')
        || (ch >= '0' && ch <= '9')
        || (ch == '_');
}