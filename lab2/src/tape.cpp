#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include "tape.h"
#include "tokenizer.h"

Tape::Tape(const std::filesystem::path& path)
    : m_path(path)
{

    if (!std::filesystem::exists(path))
    {
        std::string err = path.string() + " doesn't exist";
        throw std::logic_error(err.c_str());
    }

    m_index = 0;
    std::string buf;
    std::string content;
    std::ifstream fin(path);

    while (std::getline(fin, buf))
    {
        content += buf;
    }

    Tokenizer tokenizer(content);

    bool symbol_next = true;
    while (true)
    {
        // symbol
        auto token = tokenizer.get();
        if (symbol_next)
        {
            if (token.kind == Tokenizer::Symbol)
            {
                m_symbols.push_back(std::move(token.content));
                symbol_next = false;
            }
            else if (token.kind == Tokenizer::Comma)
            {
                m_symbols.push_back(" ");
            }
            else if (token.kind == Tokenizer::Semicolon)
            {
                m_symbols.push_back(" ");
                break;
            }
        }
        else
        {
            if (token.kind == Tokenizer::Comma)
            {
                symbol_next = true;
            }
            else if (token.kind == Tokenizer::Semicolon)
            {
                break;
            }
        }

        // if (token.kind != Tokenizer::Symbol)
        // {
        //     throw std::logic_error("syntax error");
        // }
        // m_symbols.push_back(std::move(token.content));

        // // , or ;
        // token = tokenizer.get();
        // if (token.kind  == Tokenizer::Colon)
        // {
        //     continue;
        // }
        // else if (token.kind == Tokenizer::Semicolon)
        // {
        //     break;
        // }
    }
    if (tokenizer.get().kind != Tokenizer::End)
    {
        throw std::logic_error("syntax error");
    }

    m_length = m_symbols.size();
}

void Tape::show()
{
    std::cout << "tape: " << std::endl;
    for (const auto & it: m_symbols)
    {
        std::cout << it << " ";
    }
    std::cout << std::endl;
}

const Tape::Symbol& Tape::read()
{
    return m_symbols[m_index];
}

void Tape::write(const Tape::Symbol& symbol)
{
    m_symbols[m_index] = symbol;
}

void Tape::shift(char ch)
{
    switch (ch)
    {
    case 'R':
        if (m_index == m_length-1)
        {
            m_symbols.push_back(std::string{" "});
            ++m_length;
        }
        ++m_index;
        break;

    case 'L':
        if (m_index > 0)
        {
            --m_index;
        }
        break;

    case '-':
        break;
    default:
        std::logic_error("Unknown action");
    }
}

void Tape::save()
{
    std::ofstream fout(m_path);
    if (m_symbols.size() == 0)
    {
        return;
    }
    fout << m_symbols[0];
    for (std::size_t i = 1; i < m_symbols.size(); ++i)
    {
        fout << ", " << m_symbols[i];
    }
    fout << ";";
}