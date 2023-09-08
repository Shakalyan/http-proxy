#include "bwriter.h"

#include <string.h>

Utils::BWriter::BWriter(unsigned int MTU, unsigned int tuCount)
        : MTU(MTU), bufferSize(MTU * tuCount), buffer(new char[MTU*tuCount]) {
}

Utils::BWriter::~BWriter() {
}

bool Utils::BWriter::read(char *dst) {
    if (!readAllowed) {
        return false;
    }
    memcpy(dst, buffer.get()+readOfs, MTU);
    readOfs = (readOfs + MTU) % bufferSize;
    writeAllowed = true;
    if (readOfs == writeOfs) {
        readAllowed = false;
    }
    return true;
}

bool Utils::BWriter::write(char *data) {
    if (!writeAllowed) {
        return false;
    }
    memcpy(buffer.get()+writeOfs, data, MTU);
    writeOfs = (writeOfs + MTU) % bufferSize;
    readAllowed = true;
    if (writeOfs == readOfs) {
        writeAllowed = false;
    }
    return true;
}

