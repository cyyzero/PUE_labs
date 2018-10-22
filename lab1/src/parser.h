#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "tokenizer.h"
#include "symbol_table.h"

std::size_t calculate_length(const std::string& type);
bool is_pointer(const std::string& type);
bool is_array(const std::string& type);
bool is_function(const std::string& type);

Entry parse_decl_or_use(const std::string& line);
void parse_assign(const std::string& lhs, const std::string& rhs);
class Use_parser
{
public:
    Use_parser(Tokenizer& tokenizer);
    ~Use_parser() = default;

    const Entry& get_symbol() const;
    void parse();
private:
    void recursive_parse();
    Tokenizer& m_tokenizer;
    Entry m_entry;
};

class Decl_parser
{
public:
    Decl_parser(Tokenizer& tokenizer)
        : m_tokenizer(tokenizer)
    {
    }

    ~Decl_parser() = default;

    void parse();

    const std::string& type() const;
    const std::string& identifer() const;
    std::size_t length() const;

private:
    void recursive_parse();
    void parse_parameter();

    Tokenizer& m_tokenizer;
    std::string m_identifer;
    std::string m_type;
    std::size_t m_length;
};

class Array_parser
{
public:
    Array_parser(Tokenizer& tokenizer)
        : m_tokenizer(tokenizer) 
    {
    }
    ~Array_parser() = default;

    void parse();
    std::vector<std::string> get_elems();
private:
    Tokenizer& m_tokenizer;
    std::vector<std::string> m_numbers;
};

#endif // PARSER_H