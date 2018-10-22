#include <iostream>
#include <string>
#include <cstring>
#include <assert.h>
#include "parser.h"
#include "tokenizer.h"
#include "symbol_table.h"
#include "memory.h"

extern Symbol_table g_symbol_table;
extern Memory g_memory;



bool is_pointer(const std::string& type)
{
    return type[0] == '*';
}

bool is_array(const std::string& type)
{
    return type[0] == 'A';
}

bool is_function(const std::string& type)
{
    return type[0] == 'F';
}

Entry parse_decl_or_use(const std::string &line)
{
    Tokenizer tokenizer(line);
    Entry entry;

    if (tokenizer.get().first == Tokenizer::Type)
    {
        Decl_parser decl_parser(tokenizer);

        tokenizer.reset();
        decl_parser.parse();
        if (decl_parser.identifer().size() == 0)
        {
            throw std::logic_error("syntax");
        }
        auto address = g_memory.insert(decl_parser.length());
        entry = {decl_parser.identifer(), address,
                 decl_parser.type(), decl_parser.length()};
        g_symbol_table.insert(entry);
        if (is_function(entry.type))
        {
            g_functions[entry.address] = line;
        }
    }
    else
    {
        Use_parser use_parser(tokenizer);
        tokenizer.reset();
        use_parser.parse();
        entry = use_parser.get_symbol();
    }
    return entry;
}

void parse_assign(const std::string &lhs, const std::string &rhs)
{
    Tokenizer rhs_tokenizer(rhs);
    Entry lhs_entry = parse_decl_or_use(lhs);

    switch (rhs_tokenizer.get().first)
    {
    case Tokenizer::Address:
    {
        Use_parser rhs_parser(rhs_tokenizer);
        rhs_parser.parse();
        auto rhs_entry = rhs_parser.get_symbol();
        if (lhs_entry.type != (std::string("*") + rhs_entry.type))
        {
            throw std::logic_error("type not qualified");
        }
        *reinterpret_cast<void **>(&g_memory[lhs_entry.address]) = reinterpret_cast<void *>(rhs_entry.address);
        break;
    }
    case Tokenizer::LCurly:
    {
        if (!is_array(lhs_entry.type))
        {
            std::string err = lhs_entry.name + " is not array";
            // std::cout <<  << std::endl;
            throw std::logic_error(err.c_str());
        }
        auto it = lhs_entry.type.begin();
        while (*it != '_')
            ++it;
        auto elem_len = stoul(std::string(lhs_entry.type.begin() + 5, it));
        auto elem_type = std::string(it + 1, lhs_entry.type.end());

        Array_parser array_parser(rhs_tokenizer);
        array_parser.parse();
        auto numbsers = array_parser.get_elems();
        if (numbsers.size() > elem_len)
        {
            throw std::logic_error("array out of range");
        }
        switch (elem_type[0])
        {
        /*    "int",
                "short",
                "long",
                "double",
                "float",
                "char",
                "void"  */
        case 'i':
            for (size_t i = 0; i < numbsers.size(); ++i)
            {
                *reinterpret_cast<int *>(&g_memory[lhs_entry.address] + i * sizeof(int)) =
                    stoi(numbsers[i]);
            }
            break;
        case 's':
            for (size_t i = 0; i < numbsers.size(); ++i)
            {
                *reinterpret_cast<short *>(&g_memory[lhs_entry.address] + i * sizeof(short)) =
                    static_cast<short>(stoi(numbsers[i]));
            }
            break;
        case 'l':
            for (size_t i = 0; i < numbsers.size(); ++i)
            {
                *reinterpret_cast<long *>(&g_memory[lhs_entry.address] + i * sizeof(long)) =
                    stol(numbsers[i]);
            }
            break;
        case 'd':
            for (size_t i = 0; i < numbsers.size(); ++i)
            {
                *reinterpret_cast<double *>(&g_memory[lhs_entry.address] + i * sizeof(double)) =
                    stod(numbsers[i]);
            }
            break;
        case 'f':
            for (size_t i = 0; i < numbsers.size(); ++i)
            {
                *reinterpret_cast<float *>(&g_memory[lhs_entry.address] + i * sizeof(float)) =
                    stof(numbsers[i]);
            }
            break;
        case 'c':
            for (size_t i = 0; i < numbsers.size(); ++i)
            {
                *reinterpret_cast<char *>(&g_memory[lhs_entry.address] + i * sizeof(char)) =
                    static_cast<char>(stoi(numbsers[i]));
            }
            break;
        default:
        {
            std::string err = elem_type + " don't support assnign";
            throw std::logic_error(err.c_str());
        }
        }
        break;
    }
    case Tokenizer::Number:
    {
        auto number = rhs_tokenizer.get().second;
        rhs_tokenizer.next();
        if (rhs_tokenizer.get().first != Tokenizer::End)
        {
            throw std::logic_error("syntax error");
        }
        switch (lhs_entry.type[0])
        {
        /*
        "int",
        "short",
        "long",
        "double",
        "float",
        "char",
        "void"
        */
        case 'i':
            *reinterpret_cast<int *>(&g_memory[lhs_entry.address]) = stoi(number);
            break;
        case 's':
            *reinterpret_cast<short *>(&g_memory[lhs_entry.address]) = static_cast<short>(stoi(number));
            break;
        case 'l':
            *reinterpret_cast<long *>(&g_memory[lhs_entry.address]) = stol(number);
            break;
        case 'd':
            *reinterpret_cast<double *>(&g_memory[lhs_entry.address]) = stod(number);
            break;
        case 'f':
            *reinterpret_cast<float *>(&g_memory[lhs_entry.address]) = stof(number);
            break;
        case 'c':
            *reinterpret_cast<char *>(&g_memory[lhs_entry.address]) = static_cast<char>(stoi(number));
            break;
        case 'v':
        default:
            std::string err = lhs_entry.name + " can't be assigned a number";
            // std::cout << lhs_entry.name << " can't be assigned a number" << std::endl;
            throw std::logic_error(err.c_str());
        }
        break;
    }
    default:
    {
        Use_parser use_parser(rhs_tokenizer);

        rhs_tokenizer.reset();
        use_parser.parse();
        auto &&rhs_entry = use_parser.get_symbol();
        if (rhs_entry.type == lhs_entry.type)
        {
            memcpy(reinterpret_cast<void *>(&g_memory[lhs_entry.address]),
                   reinterpret_cast<void *>(&g_memory[rhs_entry.address]),
                   rhs_entry.length);
        }
        else
        {
            throw std::logic_error("can't assign bwtween two different type");
        }
    }
    }
}

// std::size_t array_bound(const std::string& type)
// {
//     assert(type[0] == 'A');

// }

// class Use_parser
Use_parser::Use_parser(Tokenizer &tokenizer)
    : m_tokenizer(tokenizer)
{
}

const Entry &Use_parser::get_symbol() const
{
    return m_entry;
}

void Use_parser::parse()
{
    m_tokenizer.next();
    recursive_parse();
    //m_length = calculate_length(m_type);
}

void Use_parser::recursive_parse()
{
    auto token = m_tokenizer.get();
    if (token.first == Tokenizer::End)
        return;
    switch (token.first)
    {
    case Tokenizer::Type:
        throw std::logic_error("Error");
    case Tokenizer::Identifier:
        m_entry = g_symbol_table.find(token.second);
        if (m_entry == Symbol_table::nops)
        {
            std::string err = token.second + " is not in the symbol table.";
            throw std::logic_error(err.c_str());
        }
        m_tokenizer.next();
        break;
    case Tokenizer::LBraket:
        m_tokenizer.next();
        recursive_parse();
        if (m_tokenizer.get().first != Tokenizer::RBraket)
        {
            throw std::logic_error("synctax error");
        }
        m_tokenizer.next();
        break;
    // case Tokenizer::Address:
    //     m_tokenizer.next();
    //     recursive_parse();
    //     m_entry.name = std::string("&") + m_entry.name;
    //     m_entry.type = std::string() +
    case Tokenizer::Star:
        m_tokenizer.next();
        recursive_parse();
        if (!is_pointer(m_entry.type))
        {
            std::string err = m_entry.name + "is not an pointer";
            throw std::logic_error(err.c_str());
        }
        m_entry.name = std::string("*") + m_entry.name;
        m_entry.type = std::string(m_entry.type.begin() + 1, m_entry.type.end());
        m_entry.address = *reinterpret_cast<std::size_t *>(&g_memory[m_entry.address]);
        m_entry.length = calculate_length(m_entry.type);
        break;
    default:
        break;
    }
    token = m_tokenizer.get();
    if (token.first == Tokenizer::LSquare)
    {
        m_tokenizer.next();
        auto number = m_tokenizer.get();
        if (number.first != Tokenizer::Number)
        {
            throw std::logic_error("synctax error");
        }
        m_tokenizer.next();
        if (m_tokenizer.get().first != Tokenizer::RSquare)
        {
            throw std::logic_error("synctax error");
        }
        m_tokenizer.next();
        if (!is_array(m_entry.type))
        {
            std::string err = m_entry.name + " is not array";
            throw std::logic_error(err.c_str());
        }
        auto it = m_entry.type.begin();
        while (*it != '_')
            ++it;
        //auto len = stoul(std::string(type.begin()+5, it));
        auto len = stoul(std::string(m_entry.type.begin() + 5, it));
        auto elem_type = std::string(it + 1, m_entry.type.end());
        if (stoul(number.second) >= len)
        {
            throw std::logic_error("index of array is out of range");
        }
        m_entry.type = elem_type;
        m_entry.name = m_entry.name + "[" + number.second + "]";
        m_entry.length = calculate_length(m_entry.type);
        m_entry.address = m_entry.address + stoi(number.second) * m_entry.length;
    }
    recursive_parse();
}

// Class Decl_parser

// member functions

const std::string &Decl_parser::type() const
{
    return m_type;
}
const std::string &Decl_parser::identifer() const
{
    return m_identifer;
}

std::size_t Decl_parser::length() const
{
    return m_length;
}

void Decl_parser::parse()
{
    // m_tokenizer.reset();
    m_tokenizer.next();
    recursive_parse();
    m_length = calculate_length(m_type);
}

std::size_t calculate_length(const std::string &type)
{
    switch (type[0])
    {
    /*    "int",
        "short",
        "long",
        "double",
        "float",
        "char",
        "void"  */
    case 'i':
        return sizeof(int);
    case 's':
        return sizeof(short);
    case 'l':
        return sizeof(long);
    case 'f':
        return sizeof(float);
    case 'c':
        return sizeof(char);
    case 'd':
        return sizeof(double);
    case 'v':
        return 1;
    case '*':
        return sizeof(void *);
    case 'F':
        return type.length();
    case 'A':
    {
        auto it = type.begin();
        while (*it != '_')
            ++it;
        //auto len = stoul(std::string(type.begin()+5, it));
        return stoul(std::string(type.begin() + 5, it)) *
               calculate_length(std::string(it + 1, type.end()));
    }
    default:
        std::cout << type[0] << std::endl;
        throw std::logic_error("Unknown type");
    }
}

void Decl_parser::recursive_parse()
{
    auto token = m_tokenizer.get();

    if (token.first == Tokenizer::End)
        return;

    switch (token.first)
    {
    case Tokenizer::Type:
        m_tokenizer.next();
        recursive_parse();
        if (m_tokenizer.get().first == Tokenizer::LSquare)
        //            m_tokenizer.get().first == Tokenizer::LBraket)
        {
            recursive_parse();
        }
        m_type += token.second;
        // std::cout << token.second[0];
        break;
    case Tokenizer::LBraket:
        m_tokenizer.next();
        recursive_parse();
        if (m_tokenizer.get().first != Tokenizer::RBraket)
        {
            throw std::logic_error("synctax error");
        }
        m_tokenizer.next();
        break;
    case Tokenizer::Star:
        m_tokenizer.next();
        recursive_parse();
        // std::cout << "pointer to ";
        m_type += "*";
        break;
    case Tokenizer::Identifier:
        // m_identifer = token.second;
        m_identifer = std::move(token.second);
        m_tokenizer.next();
        break;
    default:
        break;
    }

    token = m_tokenizer.get();

    switch (token.first)
    {
    case Tokenizer::LSquare:
    {
        m_tokenizer.next();
        auto number = m_tokenizer.get();
        if (number.first != Tokenizer::Number)
        {
            throw std::logic_error("syntax error");
        }
        m_tokenizer.next();
        if (m_tokenizer.get().first != Tokenizer::RSquare)
        {
            throw std::logic_error("syntax error");
        }
        m_tokenizer.next();
        // std::cout << "array with length " << number.second << " of ";
        m_type = m_type + "Array" + number.second + "_";
        break;
    }
    case Tokenizer::LBraket:
        //std::cout << "function accepting ";
        m_type += "Func(";
        m_tokenizer.next();
        parse_parameter();
        m_type += ")";
        if (m_tokenizer.get().first != Tokenizer::RBraket)
        {
            throw std::logic_error("syntax error");
        }
        m_tokenizer.next();
        m_type += "->";
        break;
    default:
        break;
    }
}

void Decl_parser::parse_parameter()
{
    while (m_tokenizer.get().first != Tokenizer::RBraket)
    {
        recursive_parse();
        if (m_tokenizer.get().first == Tokenizer::Comma)
        {
            m_tokenizer.next();
        }
        if (m_tokenizer.get().first == Tokenizer::End)
        {
            throw std::logic_error("syntax error");
        }
    }
}

// class Array_parser
void Array_parser::parse()
{
    //auto token = m_tokenizer.get();
    m_tokenizer.next();
    auto token = m_tokenizer.get();
    if (token.first != Tokenizer::Number)
    {
        throw std::logic_error("syntax error");
    }
    m_numbers.push_back(std::move(token.second));
    m_tokenizer.next();
    while (m_tokenizer.get().first != Tokenizer::RCurly)
    {
        token = m_tokenizer.get();
        if (token.first != Tokenizer::Comma)
        {
            throw std::logic_error("syntax error");
        }
        m_tokenizer.next();
        auto number = m_tokenizer.get();
        if (number.first != Tokenizer::Number)
        {
            throw std::logic_error("syntax error");
        }
        m_numbers.push_back(std::move(number.second));
        m_tokenizer.next();
    }
    m_tokenizer.next();
    if (m_tokenizer.get().first != Tokenizer::End)
    {
        throw std::logic_error("synatax error");
    }
}

std::vector<std::string> Array_parser::get_elems()
{
    return m_numbers;
}