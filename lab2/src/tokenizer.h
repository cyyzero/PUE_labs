#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include <string>
#include <cstdint>

class Tokenizer
{
public:
    enum Kind : std::uint8_t
    {
        Q, Sigma, q0, Accept, Reject, Delta,
        Colon, Comma, Semicolon, LBraket, RBraket, Map,
        Symbol,
        End
    };

    struct Token
    {
        Token(Kind k, std::string&& c)
            : kind(k), content(std::move(c)) { }
        Kind kind;
        std::string content;
    };


    Tokenizer(const std::string& config)
        : m_config(config)
    {
        m_current = m_config.begin();
    }

    ~Tokenizer() = default;

    Token get();

private:
    const std::string m_config;
    std::string::const_iterator m_current;
};

#endif