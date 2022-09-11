#pragma once


#include <string>

/*
 * tags consists of [5, 32] ascii letters: a-zA-Z0-9_
 */

class Tag: public std::string{
public:
    static constexpr auto MinLength = 5;
    static constexpr auto MaxLength = 32;
public:
    Tag(std::string Tag);

    Tag(Tag &&other) = default;

    Tag &operator=(Tag &&other) = default;

    bool IsValid()const;

    operator bool()const{
        return IsValid();
    }
};