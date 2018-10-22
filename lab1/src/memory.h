#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <string>

void print_memory(const std::string& type, std::size_t address);

struct Memory
{
public:
    Memory()
        : m_index(0) { }

    uint8_t& operator[](std::size_t address);
    std::size_t insert(std::size_t size);
    void show() const;
private:
    constexpr static std::size_t MAX_LEN = 1024;
    uint8_t m_mem[MAX_LEN];
    std::size_t m_index;
};

#endif // MEMORY_H