#ifndef _TURING_MACHINE_H_
#define _TURING_MACHINE_H_

#include <string>
#include <utility>
#include <filesystem>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include "tape.h"

namespace std
{
template<>
struct hash<pair<string, string>>
{
    size_t operator()(const pair<string, string>& p) const
    {
        return hash<string>()(p.first + p.second);
    }
};
}

class Turing_machine
{
public:
    using State  = std::string;
    using Symbol = std::string;
    using Action = char;

    Turing_machine(const std::filesystem::path& config_paht,
                   const std::filesystem::path& tape_path);

    void run();
    void show();
    void accept();
    void reject();
    void halt();

private:
    std::unordered_set<State> Q;
    std::unordered_set<Symbol> Sigma;
    State q0;
    std::unordered_set<State> Accept;
    std::unordered_set<State> Reject;
    std::unordered_map<std::pair<State, Symbol>, std::tuple<State, Symbol, Action>> Delta;

    Tape tape;
};

#endif
