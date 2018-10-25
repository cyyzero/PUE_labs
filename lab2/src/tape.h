#ifndef _TAPE_H_
#define _TAPE_H_

#include <string>
#include <vector>
#include <filesystem>

class Tape
{
public:
    using Symbol = std::string;

    Tape(const std::filesystem::path& path);
    ~Tape() = default;

    void shift(char);
    const Symbol& read();
    void write(const Symbol& symbol);
    void show();
    void save();

private:
    std::vector<Symbol> m_symbols;
    std::size_t m_index;
    std::size_t m_length;
    std::string m_path;
};

#endif