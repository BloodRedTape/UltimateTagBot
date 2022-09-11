#include "keytag.hpp"
#include "utils.hpp"


Keytag::Keytag(std::string keytag){
    if(!keytag.size() || CountChars('@', keytag) || CountChars('#', keytag))
        return;
    
    assign(ToLowerCase(keytag));
}

bool Keytag::IsValid()const{
    return size(); 
}