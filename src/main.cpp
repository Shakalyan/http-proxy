#include "logger/logger.h"
#include "proxy/proxy.h"
#include "proxy/exception.h"
#include "utils/rwbuffer.h"

#include <iostream>
#include <fstream>

int main() {
    Logger::StdIOLogger logger(std::cout);

    try {
        Proxy::ProxyServer proxy(3333, logger);
        proxy.start();
    } catch (Proxy::ProxyException &exception) {
        logger.log(Logger::INFO_LEVEL, "Terminating...");
    }

    return 0;
}