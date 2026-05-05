#ifndef API_HANDLER_H
#define API_HANDLER_H

#include <string>
#include <vector>
#include <curl/curl.h>

// Include json.hpp dari folder lib lokal
#include "../lib/json.hpp"

using json = nlohmann::json;

struct ChatMessage {
    std::string role;
    std::string content;
};

class APIHandler {
private:
    std::string apiKey;
    std::string apiUrl;
    std::string model;
    CURL* curl;
    
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);
    std::string makeRequest(const std::string& requestBody);
    json parseResponse(const std::string& response);
    
public:
    APIHandler(const std::string& apiKey, const std::string& apiUrl, const std::string& model);
    ~APIHandler();
    
    std::string sendMessage(const std::vector<ChatMessage>& messages, 
                           const std::string& systemPrompt = "");
    bool testConnection();
};

#endif