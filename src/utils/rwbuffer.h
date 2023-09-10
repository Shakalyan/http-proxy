#ifndef RWBUFFER_GUARD
#define RWBUFFER_GUARD

#include <vector>
#include <memory>

namespace Utils {

    class RWBuffer {
    private:
        size_t bufLineSize;
        int pointerLine;
        int pointerOfs;
        std::vector<std::unique_ptr<char[]>> buffer;
        int bufferSize;

    public:

        RWBuffer(RWBuffer const&) = delete;
        RWBuffer& operator= (RWBuffer const&) = delete;

        explicit RWBuffer(size_t bufLineSize);
        RWBuffer(RWBuffer&& rwBuffer);
        RWBuffer& operator= (RWBuffer&& RWBuffer);
        ~RWBuffer();

        virtual void read(char *dst, int nbytes);
        virtual void write(char *data, int nbytes);
        virtual void resetPointer();
        virtual int getBufferSize() const;
    
    private:
        void addNewLine();
        int ptrTo1D();

    };

}

#endif