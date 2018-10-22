#include "symbol_table.h"
#include "memory.h"
#include <stdexcept>
#include <iostream>
// #define ROUND_UP(address, align) ((address + align - 1) & ~(align-1))

Symbol_table g_symbol_table;
std::unordered_map<std::size_t, std::string> g_functions;

Entry Symbol_table::nops{};

bool operator==(const Entry& lhs, const Entry& rhs)
{
    return lhs.name == rhs.name;
}

std::ostream& operator<<(std::ostream& in, const Entry& item)
{
    in << item.name << " " << item.type << " " << item.length << " " << item.address;
    // print_memory(item.type, item.length);
    return in;
}

const Entry& Symbol_table::find(const std::string& sym) const
{
    for (const auto& item: m_table)
    {
        if (item.name == sym)
            return item;
    }
    return nops;
}

Entry& Symbol_table::find(const std::string& sym)
{
    for (auto& item: m_table)
    {
        if (item.name == sym)
            return item;
    }
    return nops;
}

void Symbol_table::insert(const Entry& entry)
{
    if (find(entry.name) == nops)
    {
        m_table.push_back(entry);
        return;
    }
    std::string err = entry.name + " has been in the symbol table";
    throw std::logic_error(err.c_str());
}

void Symbol_table::show() const
{
    for (const auto& item: m_table)
    {
        std::cout << item << " | ";
        print_memory(item.type, item.address);
        std::cout << std::endl;
    }
}