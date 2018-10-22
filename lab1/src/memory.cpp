#include <stdexcept>
#include <iostream>
#include <cstdio>
#include "memory.h"
#include "parser.h"

Memory g_memory;

uint8_t& Memory::operator[](std::size_t index)
{
    if (index >= MAX_LEN)
        throw std::out_of_range("Memory index out of range");
    return m_mem[index];
}

void Memory::show() const
{
    const std::size_t LEN = 16;
    for (std::size_t i = 0; i < MAX_LEN; i += LEN)
    {
        printf("%03lx ", i);
        for (std::size_t j = 0; j < LEN; j += 2)
        {
            printf(" %02x%02x", m_mem[i+j], m_mem[i+j+1]);
        }
        printf("\n");
    }
}

std::size_t Memory::insert(std::size_t size)
{
    // if (size == 1)
    //     size = 2;
    std::size_t len = ((size + 4 - 1) & (~(4 - 1)));
    auto ret = m_index;
    m_index += len;
    return ret;
}

void print_memory(const std::string& type, std::size_t address)
{
    switch (type[0])
    {
    /*    "int",
        "short",
        "long",
        "double",
        "float",
        "char",
        "void" */
    case 'F':
    {
        // std::string 
        printf("0x%lx ", address);
        // std::cout << address << std::endl;
        break;
    }
    case '*':
        std::cout << *reinterpret_cast<void**>(&g_memory[address]) << " ";
        break;
    case 'i':
        std::cout << *reinterpret_cast<int*>(&g_memory[address]) << " ";
        break;
    case 's':
        std::cout << *reinterpret_cast<short*>(&g_memory[address]) << " ";
        break;
    case 'l':
        std::cout << *reinterpret_cast<long*>(&g_memory[address]) << " ";
        break;
    case 'd':
        std::cout << *reinterpret_cast<double*>(&g_memory[address]) << " ";
        break;
    case 'f':
        std::cout << *reinterpret_cast<float*>(&g_memory[address]) << " ";
        break;
    case 'c':
        std::cout << *reinterpret_cast<char*>(&g_memory[address]) << " ";
        break;
    case 'v':
        std::cout << " ";
        break;
    case 'A':
    {
        auto it = type.begin();
        while (*it != '_')
            ++it;
        auto len = stoul(std::string(type.begin()+5, it));
        for (decltype(len) i = 0; i < len; ++i)
        {
            auto str = std::string(it+1, type.end());
            print_memory(str, address + i * calculate_length(str));
        }
        break;
    }
    default:
        throw std::logic_error("Unknown type");
    }
}