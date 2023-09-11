#ifndef HTTPREQUEST_GUARD
#define HTTPREQUEST_GUARD

#include "httpmethod.h"
#include "httpheader.h"
#include <string>
#include <vector>

namespace HTTP {

    class HttpRequest {
    private:
        HttpMethod method;
        std::string dstHost;
        int dstPort;
        std::string urn;
        std::vector<HttpHeader> headers;
        std::string content;

    public:

        HttpRequest(HttpMethod method, 
                    std::string const& dstHost,
                    int dstPort,
                    std::string const& urn, 
                    std::vector<HttpHeader> const& headers, 
                    std::string const& content);
        
        HttpRequest(HttpMethod method, 
                    std::string&& dstHost,
                    int dstPort, 
                    std::string&& urn, 
                    std::vector<HttpHeader>&& headers, 
                    std::string&& content);
        
        HttpRequest(HttpRequest&& request);
        HttpRequest& operator= (HttpRequest&& request);
        
        virtual ~HttpRequest();

        friend HttpRequest parseHttpRequest(std::string const& data);

        std::string getDstHost() const;
        int getDstPort() const;

    };

    HttpRequest parseHttpRequest(std::string const& data);

}

#endif