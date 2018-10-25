// #include <unistd.h>
#include <fstream>
#include <iostream>
#include "tokenizer.h"
#include "turing_machine.h"


// Turing_machine

Turing_machine::Turing_machine(const std::filesystem::path& config_path,
                               const std::filesystem::path& tape_path)
    : tape(tape_path)
{
    std::string line;
    std::string config;
    std::ifstream fin(config_path);
    while (std::getline(fin, line))
    {
        config += line;
    }

    Tokenizer tokenizer(config);
    // std::cout << config << std::endl;
    static auto insert = [&tokenizer, &Q = this->Q] (std::unordered_set<std::string>& set,
                                       bool check = false) {
        auto token = tokenizer.get();
        if (token.kind != Tokenizer::Colon)
        {
            // std::cout << token.content << std::endl;
            throw std::logic_error("syntax error 21");
        }
        while (true)
        {
            token = tokenizer.get();
            if (token.kind != Tokenizer::Symbol)
            {
                if (token.kind == Tokenizer::Semicolon)
                {
                    // std::cout << "Fck" << std::endl;
                    return;
                }
                // std::cout << token.content << std::endl;
                throw std::logic_error("syntax error 22");
            }
            if (check)
            {
                if (Q.find(token.content) == Q.end())
                {
                    std::string err = token.content + " doesn't in Q";
                    throw std::logic_error(err.c_str());
                }
            }
            set.insert(std::move(token.content));
            token = tokenizer.get();
            if (token.kind == Tokenizer::Comma)
            {
                continue;
            }
            else if (token.kind == Tokenizer::Semicolon)
            {
                break;
            }
            else
            {
                throw std::logic_error("syntax error 23");
            }
        }
    };

    Sigma.insert(" ");
    auto token = tokenizer.get();
    while (token.kind != Tokenizer::End)
    {
        // Q, Sigma, q0, Accept, Reject, Delta,
        // Colon, Comma, Semicolon, LBraket, RBraket, Map,
        // Symbol,

        switch (token.kind)
        {
        case Tokenizer::Q:
            // std::cout << "Q :";
            insert(Q);
            // for (const auto &q: Q)
            // {
            //     std::cout << q << " ";
            // }
            // std::cout << std::endl;
            break;

        case Tokenizer::Sigma:
            // std::cout << "Sigma: ";
            insert(Sigma);
            // for (const auto &q: Sigma)
            // {
            //     std::cout << q << " ";
            // }
            // std::cout << std::endl;
            break;

        case Tokenizer::Accept:
            // std::cout << "Accept: ";
            insert(Accept, true);
            // for (const auto &q: Accept)
            // {
            //     std::cout << q << " ";
            // }
            // std::cout << std::endl;
            break;

        case Tokenizer::Reject:
            // std::cout << "Reject: ";
            insert(Reject, true);
            // for (const auto &q: Reject)
            // {
            //     std::cout << q << " ";
            // }
            // std::cout << std::endl;
            break;

        case Tokenizer::q0:
            // :
            if (tokenizer.get().kind != Tokenizer::Colon)
            {
                throw std::logic_error("syntax error 1");
            }

            // state
            token = tokenizer.get();
            if (token.kind != Tokenizer::Symbol)
            {
                throw std::logic_error("syntax error 1");
            }
            // guarantee that token.content is in Q
            if (Q.find(token.content) == Q.end())
            {
                std::string err = token.content + " doesn't in Q";
                throw std::logic_error(err.c_str());
            }
            q0 = std::move(token.content);

            // ;
            if (tokenizer.get().kind != Tokenizer::Semicolon)
            {
                throw std::logic_error("syntax error 2");
            }
            // std::cout << "q0: " << q0 << std::endl;
            break;

        case Tokenizer::Delta:
        {
            // :
            if (tokenizer.get().kind != Tokenizer::Colon)
            {
                throw std::logic_error("syntax error 3");
            }
            while (true)
            {
                // (
                if (tokenizer.get().kind != Tokenizer::LBraket)
                {
                    throw std::logic_error("syntax error 4");
                }

                // state
                token = tokenizer.get();
                if (token.kind != Tokenizer::Symbol)
                {
                    throw std::logic_error("syntax error 5");
                }
                if (Q.find(token.content) == Q.end())
                {
                    std::string err = token.content + " doesn't in Q";
                    throw std::logic_error(err.c_str());
                }
                State state_from = std::move(token.content);

                // ,
                if (tokenizer.get().kind != Tokenizer::Comma)
                {
                    throw std::logic_error("syntax error 6");
                }

                // symbol or )
                Symbol symbol_from;
                token = tokenizer.get();
                if (token.kind == Tokenizer::Symbol)
                {
                    if (Sigma.find(token.content) == Sigma.end())
                    {
                        std::string err = token.content + " doesn't in Sigma";
                        throw std::logic_error(err.c_str());
                    }
                    symbol_from = std::move(token.content);

                    // )
                    if (tokenizer.get().kind != Tokenizer::RBraket)
                    {
                        throw std::logic_error("syntax error 8");
                    }
                }
                // )
                else if (token.kind == Tokenizer::RBraket)
                {
                    symbol_from = " ";
                }
                else
                {
                    std::cout << token.content << std::endl;
                    throw std::logic_error("syntax error");
                }

                // ->
                if (tokenizer.get().kind != Tokenizer::Map)
                {
                    // std::cout << token.content << std::endl;
                    throw std::logic_error("syntax error 9");
                }

                // (
                if (tokenizer.get().kind != Tokenizer::LBraket)
                {
                    throw std::logic_error("syntax error 10");
                }

                // state
                token = tokenizer.get();
                if (token.kind != Tokenizer::Symbol)
                {
                    throw std::logic_error("syntax error 11");
                }
                if (Q.find(token.content) == Q.end())
                {
                    std::string err = token.content + " doesn't in Q";
                    throw std::logic_error(err.c_str());
                }
                State state_to = std::move(token.content);

                // ,
                if (tokenizer.get().kind != Tokenizer::Comma)
                {
                    throw std::logic_error("syntax error 12");
                }

                // symbol or )
                token = tokenizer.get();
                Symbol symbol_to;
                if (token.kind == Tokenizer::Symbol)
                {
                    if (Sigma.find(token.content) == Sigma.end())
                    {
                        std::string err = token.content + " doesn't in Sigma";
                        throw std::logic_error(err.c_str());
                    }
                    symbol_to = std::move(token.content);
                    // std::cout << 
                    // )
                    if (tokenizer.get().kind != Tokenizer::Comma)
                    {
                        throw std::logic_error("syntax error 100");
                    }
                }
                // )
                else if (token.kind == Tokenizer::Comma)
                {
                    symbol_to = " ";
                }
                else
                {
                    std::cout << token.content << " f" << std::endl;
                    throw std::logic_error("syntax error");
                }

                // action
                token = tokenizer.get();
                if (token.kind != Tokenizer::Symbol)
                {
                    throw std::logic_error("syntax error 15");
                }
                if (token.content != "R" && token.content != "L" && token.content != "-")
                {
                    std::string err = token.content + " is not 'R' or 'L' or '-'";
                    throw std::logic_error(err.c_str());
                }
                Action action = token.content[0];

                // )
                if (tokenizer.get().kind != Tokenizer::RBraket)
                {
                    throw std::logic_error("syntax error 16");
                }

                Delta.insert(std::make_pair(std::make_pair(std::move(state_from), std::move(symbol_from)),
                    std::make_tuple(std::move(state_to), std::move(symbol_to), action))
                );

                // , or ;
                token = tokenizer.get();
                if (token.kind == Tokenizer::Comma)
                {
                    continue;
                }
                else if (token.kind == Tokenizer::Semicolon)
                {
                    break;
                }
                else
                {
                    throw std::logic_error("syntax error 17");
                }
                break;
            }
        }

        default:
            break;
        }
        token = tokenizer.get();
    }

    // check Accept and Reject has no intersection
    for (const auto &s : Accept)
    {
        if (Reject.find(s) != Accept.end())
        {
            throw std::logic_error("Accept and Reject don't have intersection");
        }
    }
}

void Turing_machine::run()
{
    State  current_state  = q0;
    Symbol current_symbol = tape.read();

    while (true)
    {
        // sleep(1);
        if (Accept.find(current_state) != Accept.end())
        {
            accept();
            return;
        }

        if (Reject.find(current_state) != Reject.end())
        {
            reject();
            return;
        }

        auto current = std::make_pair(current_state, current_symbol);

        std::cout << "current: " << current_state << " " << current_symbol << std::endl;

        auto next = Delta.find(current);
        if (next == Delta.end())
        {
            halt();
            return;
        }

        current_state  = std::get<0>(next->second);
        tape.write(std::get<1>(next->second));
        tape.shift(std::get<2>(next->second));

        std::cout << "next:    " << std::get<0>(next->second) << " " << std::get<1>(next->second)
                  << " " <<  std::get<2>(next->second) << std::endl;

        current_symbol = tape.read();
    }
}

void Turing_machine::show()
{
    std::cout << "config: " << std::endl;
    for (const auto& q: Q)
    {
        std::cout << q << " ";
    }
    std::cout << "\n";

    for (const auto& s: Sigma)
    {
        std::cout << s << " ";
    }
    std::cout << "\n";

    std::cout << q0 << std::endl;

    for (const auto &a : Accept)
    {
        std::cout << a << " ";
    }
    std::cout << "\n";

    for (const auto& r: Reject)
    {
        std::cout << r << " ";
    }
    std::cout << "\n";

    for (const auto& it: Delta)
    {
        std::cout << it.first.first << " " << it.first.second << " -> "
                  << std::get<0>(it.second) << " " << std::get<1>(it.second) << " " << std::get<2>(it.second) << std::endl;
    }

    tape.show();
}

void Turing_machine::accept()
{
    std::cout << "Accept!" << std::endl;

    tape.show();

    tape.save();
}

void Turing_machine::reject()
{
    std::cout << "Reject!" << std::endl;

    tape.show();

    tape.save();
}

void Turing_machine::halt()
{
    std::cout << "Runtime error!" << std::endl;

    exit(EXIT_FAILURE);
}