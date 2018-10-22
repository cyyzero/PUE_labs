#include <unordered_set>
#include <string>
#include "tokenizer.h"

void Tokenizer::reset()
{
    m_current = m_line.begin();
}

void Tokenizer::next()
{
    static std::unordered_set<std::string> types = {
        "int",
        "short",
        "long",
        "double",
        "float",
        "char",
        "void"
    };

    std::string::iterator last_pos;

    while (m_current != m_line.end())
    {
        if (isalpha(*m_current) || *m_current == '_')
        {
            last_pos = m_current;
            while (m_current != m_line.end() && (isalnum(*m_current) || *m_current == '_'))
            {
                ++m_current;
            }
            std::string token = std::string(last_pos, m_current);
            if (types.find(token) != types.end())
            {
                m_token =  {Type, token};
                return;
            }
            else
            {
                m_token =  {Identifier, token};
                return;
            }
        }
        else if (isdigit(*m_current))
        {
            last_pos = m_current;
            while (m_current != m_line.end() && (isdigit(*m_current) || *m_current == '.'))
            {
                ++m_current;
            }
            m_token =  {Number, std::string(last_pos, m_current)};
            return;
        }
        else if (isspace(*m_current))
        {
            ++m_current;
        }
        else
        {
            int ch = *m_current;
            ++m_current;
            switch(ch)
            {
            case '[':
                m_token =  {LSquare, "["};
                return;
            case ']':
                m_token =  {RSquare, "]"};
                return;
            case '(':
                m_token =  {LBraket, "("};
                return;
            case ')':
                m_token =  {RBraket, ")"};
                return;
            case '{':
                m_token = {LCurly, "{"};
                return;
            case '}':
                m_token = {RCurly, "}"};
                return;
            case ',':
                m_token =  {Comma, ","};
                return;
            case '*':
                m_token =  {Star, "*"};
                return;
            case '&':
                m_token =  {Address, "&"};
                return;
            default:
                throw std::logic_error("Bad token");
            }
        }
    }
    m_token =  {End, std::string()};
}

Tokenizer::Token Tokenizer::get()
{
    return m_token;
}