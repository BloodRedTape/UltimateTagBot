#pragma once

#include <string>

class ArgsIterator{
    static constexpr char Separator = ' ';
private:
    const char *m_CommandLine;
    const char *m_Start;
    const char *m_End;
public:
    ArgsIterator(const char *cmd_line);

    bool IsValid()const;

    operator bool()const{
        return IsValid();
    }

    void Advance();

    ArgsIterator &operator++(){
        Advance();
        return *this;
    }

    std::string Current();
private:
    static const char *FindArgEnd(const char *start);

    static const char *FindArgStart(const char *line);
};