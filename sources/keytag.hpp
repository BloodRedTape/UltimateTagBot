#pragma once


#include <string>

/*
 * Keytag consists of >=1 Unicode characters, except @ and #
 */

class Keytag: public std::string{
public:
    Keytag(std::string keytag);

    Keytag(Keytag &&other) = default;

    Keytag &operator=(Keytag &&other) = default;

    bool IsValid()const;

    operator bool()const{
        return IsValid();
    }
};