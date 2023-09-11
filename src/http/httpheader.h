#ifndef HTTPHEADER_GUARD
#define HTTPHEADER_GUARD

#include <string>
#include <vector>

namespace HTTP {

    class HttpHeader {
    private:
        std::string key;
        std::string value;
    
    public:

        HttpHeader(std::string const& key, std::string const& value);
        HttpHeader(std::string&& key, std::string&& value);
        HttpHeader(HttpHeader const& httpHeader);
        HttpHeader(HttpHeader&& httpHeader);

        HttpHeader& operator= (HttpHeader const& httpHeader);
        HttpHeader& operator= (HttpHeader&& httpHeader);

        virtual ~HttpHeader();

        std::string getKey() const;
        std::string getValue() const;

    };

}

#endif