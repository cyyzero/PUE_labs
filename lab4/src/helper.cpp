#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    string s;
    string line;
    string format;
    ifstream fin(argv[1]);
    while (getline(fin, line))
    {
        s += (line+"\n");
    }

    for (auto ch: s) {
        switch (ch)
        {
        case '\\':
            cout << "\\\\";
            break;
        case '%':
            cout << "%%";
            break;
        case '\n':
            format += (", " + to_string(int('\n')));
            cout << "%c";
            break;
        case '\"':
            format += (", " + to_string(int('\"')));
            cout << "%c";
            break;
        case '@':
            cout << "%s";
            format += ", s";
            break;
        default:
            cout << ch;
        }
    }
    cout << endl;
    cout << format << endl;
}