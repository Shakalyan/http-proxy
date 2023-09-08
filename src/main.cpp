#include "logger/logger.h"

#include <iostream>
#include <fstream>

int main() {
    
    std::ofstream fstream;
    fstream.open("LOGFILE.txt", std::ios_base::app);
    Logger::StdIOLogger logger(fstream);

    return 0;
}