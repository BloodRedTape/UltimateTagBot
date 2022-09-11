#include "args_iterator.hpp"


ArgsIterator::ArgsIterator(const char *cmd_line):
    m_CommandLine(cmd_line)
{
    m_Start = FindArgStart(cmd_line);
    m_End = FindArgEnd(m_Start);
}

bool ArgsIterator::IsValid()const{
    return m_Start != m_End;
}

void ArgsIterator::Advance(){
    m_Start = FindArgStart(m_End);
    m_End = FindArgEnd(m_Start);
}

std::string ArgsIterator::Current(){
    return {m_Start, m_End - m_Start};
}

const char *ArgsIterator::FindArgEnd(const char *line){
    while(*line && *line != Separator)
        line++;
    return line;
}

const char *ArgsIterator::FindArgStart(const char *line){
    while(*line && *line == Separator)
        line++;
    return line;
}