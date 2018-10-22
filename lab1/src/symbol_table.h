#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

extern std::unordered_map<std::size_t, std::string> g_functions;

struct Entry
{
    std::string name;
    std::size_t address;
    std::string type;
    std::size_t length;
};

class Symbol_table
{
public:

    Symbol_table() = default;
    ~Symbol_table() = default;

    const Entry& find(const std::string&) const;
    Entry& find(const std::string&);
    void insert(const Entry&);
    void show() const;
    static Entry nops;

private:
    std::vector<Entry> m_table;
};

bool operator==(const Entry&, const Entry&);
std::ostream& operator<<(std::ostream&, const Entry& entry);
#endif