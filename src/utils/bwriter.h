#ifndef BWRITER_GUARD
#define BWRITER_GUARD

#include <memory>

namespace Utils {

    class BWriter {
    private:
        unsigned int MTU;
        size_t bufferSize;
        std::unique_ptr<char> buffer;
        unsigned long readOfs = 0;
        unsigned long writeOfs = 0;
        bool readAllowed = false;
        bool writeAllowed = true;
    
    public:
        
        BWriter(BWriter const& bwriter) = delete;
        BWriter& operator= (BWriter const& bwriter) = delete;

        BWriter(unsigned int MTU, unsigned int tuCount);
        virtual ~BWriter();

        virtual bool write(char *data);
        virtual bool read(char *dst);

    };

}

#endif