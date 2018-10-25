#include <unordered_map>
#include <iostream>
#include "tokenizer.h"

Tokenizer::Token Tokenizer::get()
{
    static const std::unordered_map<std::string, Kind> types = {
        {"Q", Q}, {"Sigma", Sigma}, {"q0", q0}, {"Accept", Accept}, {"Reject", Reject}, {"Delta", Delta}
    };


    while (m_current != m_config.end() && isspace(*m_current))
    {
        ++m_current;
    }
    if (m_current == m_config.end())
    {
        return Token(End, std::string());
    }

    if (isalnum(*m_current))
    {
        auto last = m_current;
        ++m_current;
        while (m_current != m_config.end() && isalnum(*m_current))
        {
            ++m_current;
        }
        std::string seq(last, m_current);
        auto it = types.find(seq);
        if (it != types.end())
        {
            return Token(it->second, std::move(seq));                  // return
        }
        else
        {
            return Token(Symbol, std::move(seq));                      // return
        }
    }
    else
    {
        switch (*m_current++)
        {
        case ':':
            return Token(Colon, ":");
        case ',':
            return Token(Comma, ",");
        case ';':
            return Token(Semicolon, ";");
        case '(':
            return Token(LBraket, "(");
        case ')':
            return Token(RBraket, ")");
        case '-':
            if (m_current == m_config.end())
            {
                std::cout << "T" << std::endl;
                throw std::logic_error("syntax error");
            }
            if (*m_current == '>')
            {
                ++m_current;
                return Token(Map, "->");
            }
            else
                return Token(Symbol, "-");
        default:
        {
            // std::cout << &(*--m_current) << std::endl;
            throw std::logic_error("syntax error");
        }
        }
    }

}
