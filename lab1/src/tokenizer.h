#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <cstdint>
#include <string>
#include <iostream>

class Tokenizer
{
public:
    enum Token_kind: uint8_t
    {
        Number, Identifier, Type,
        Address , LCurly, RCurly, LSquare, RSquare, LBraket, RBraket, Star, Comma,
        End
    };
    using Token = std::pair<Token_kind, std::string>;
public:
    Tokenizer(const std::string& line)
        : m_line(line)
    {
        m_current = m_line.begin();
        next();
    }

    ~Tokenizer() = default;

    Token get();
    void next();
    void reset();
private:
    //std::istream& m_file;
    Token m_token;
    std::string m_line;
    std::string::iterator m_current;
};

#endif // TOKENIZER_H