#include "bot.hpp"
#include <iostream>

int main(int argc, char **argv){
    if(argc != 2)
        return EXIT_FAILURE;

    UltimateTagBot bot(argv[1]);
    
    return bot.Run();
}