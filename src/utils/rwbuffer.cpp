#include "rwbuffer.h"
#include <string.h>

void Utils::RWBuffer::addNewLine() {
    buffer.push_back(std::unique_ptr<char[]>(new char[bufLineSize]));
}

int Utils::RWBuffer::ptrTo1D() {
    return pointerLine * bufLineSize + pointerOfs;
}

int Utils::RWBuffer::getBufferSize() const {
    return bufferSize;
}

Utils::RWBuffer::RWBuffer(size_t bufLineSize) 
        : bufLineSize(bufLineSize),
          bufferSize(0) {
    addNewLine();
    resetPointer();
}

Utils::RWBuffer::~RWBuffer() {

}

Utils::RWBuffer::RWBuffer(RWBuffer&& rwBuffer) 
        : bufLineSize(rwBuffer.bufLineSize),
          pointerLine(rwBuffer.pointerLine),
          pointerOfs(rwBuffer.pointerOfs),
          buffer(std::move(rwBuffer.buffer)),
          bufferSize(rwBuffer.bufferSize) {

}

Utils::RWBuffer& Utils::RWBuffer::operator= (RWBuffer&& rwBuffer) {
    bufLineSize = rwBuffer.bufLineSize;
    pointerLine = rwBuffer.pointerLine;
    pointerOfs = rwBuffer.pointerOfs;
    buffer = std::move(rwBuffer.buffer);
    bufferSize = rwBuffer.bufferSize;
    return *this;
}

void Utils::RWBuffer::resetPointer() {
    pointerLine = 0;
    pointerOfs = 0;
}

void Utils::RWBuffer::write(char *data, int nbytes) {
    int written = 0;
    while (pointerOfs + nbytes >= bufLineSize) {
        int wbytes = bufLineSize - pointerOfs;
        memcpy(buffer[pointerLine].get() + pointerOfs, data + written, wbytes);
        nbytes -= wbytes;
        written += wbytes;
        ++pointerLine;
        if (pointerLine == buffer.size()) {
            addNewLine();
        }
        pointerOfs = 0;
    }
    memcpy(buffer[pointerLine].get() + pointerOfs, data + written, nbytes);
    pointerOfs += nbytes;
    bufferSize = ptrTo1D();
}

void Utils::RWBuffer::read(char *dst, int nbytes) {
    int p1d = ptrTo1D();
    int read = 0;
    nbytes = (p1d + nbytes > bufferSize) ? bufferSize - p1d : nbytes;
    while (pointerOfs + nbytes >= bufLineSize) {
        int rbytes = bufLineSize - pointerOfs;
        memcpy(dst+read, buffer[pointerLine].get() + pointerOfs, rbytes);
        nbytes -= rbytes;
        read += rbytes;
        ++pointerLine;
        pointerOfs = 0;
    }
    memcpy(dst + read, buffer[pointerLine].get() + pointerOfs, nbytes);
    pointerOfs += nbytes;
}