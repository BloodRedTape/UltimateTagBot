#include "keytag.hpp"
#include "utils.hpp"


Keytag::Keytag(std::string keytag){
    assign(ToLowerCase(keytag));
}

bool Keytag::IsValid()const{
    return size() && !CountChars('@', c_str()) && !CountChars('#', c_str());
}