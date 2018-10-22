#include <iostream>
#include <string>
#include <exception>
#include "tokenizer.h"
#include "parser.h"
#include "symbol_table.h"
#include "memory.h"

extern Symbol_table g_symbol_table;
extern Memory g_memory;

int main()
{
    std::string line;
    std::size_t pos;
    std::cout << ">> ";
    while (std::getline(std::cin, line))
    {
        try
        {
            if (line == "sym_tab")
            {
                g_symbol_table.show();
            }
            else if (line == "memory")
            {
                g_memory.show();
            }
            else if ((pos = line.find("=")) == std::string::npos)
            {
                auto entry = parse_decl_or_use(line);
                if (is_function(entry.type))
                {
                    std::cout << g_functions[entry.address];
                }
                else
                {
                    print_memory(entry.type, entry.address);
                }
                std::cout << std::endl;
            }
            else
            {
                // auto pos = line.find("=");
                std::string lhs = std::string(line.begin(), line.begin() + pos);
                std::string rhs = std::string(line.begin()+pos+1, line.end());
                parse_assign(lhs, rhs);
            }
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
        std::cout << ">> ";
    }
    return 0;
}