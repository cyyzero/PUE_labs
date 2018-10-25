#include "turing_machine.h"

int main(int argc, char const *argv[])
{
    const char* config_path = "../config";
    const char* tape_path   = "../tape";
    
    if (argc == 3)
    {
        config_path = argv[1];
        tape_path   = argv[2];
    }
    Turing_machine machine(config_path, tape_path);
    machine.show();
    machine.run();

    return 0;
}
