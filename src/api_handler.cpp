#include "api_handler.h"
#include <stdexcept>
#include <iostream>

APIHandler::APIHandler(const std::string& apiKey, const std::string& apiUrl, 
                       const std::string& model)
    : apiKey(apiKey), apiUrl(apiUrl), model(model) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize CURL");
    }
}

APIHandler::~APIHandler() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

size_t APIHandler::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::string APIHandler::makeRequest(const std::string& requestBody) {
    std::string response;
    
    if (!curl) {
        throw std::runtime_error("CURL not initialized");
    }
    
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    std::string authHeader = "Authorization: Bearer " + apiKey;
    headers = curl_slist_append(headers, authHeader.c_str());
    
    curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestBody.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, requestBody.length());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    
    CURLcode res = curl_easy_perform(curl);
    
    curl_slist_free_all(headers);
    
    if (res != CURLE_OK) {
        throw std::runtime_error("CURL request failed: " + std::string(curl_easy_strerror(res)));
    }
    
    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    
    if (httpCode != 200) {
        throw std::runtime_error("HTTP request failed with code: " + std::to_string(httpCode) + 
                               "\nResponse: " + response);
    }
    
    return response;
}

json APIHandler::parseResponse(const std::string& response) {
    try {
        return json::parse(response);
    } catch (const json::parse_error& e) {
        throw std::runtime_error("Failed to parse JSON response: " + std::string(e.what()) + 
                               "\nResponse: " + response);
    }
}

std::string APIHandler::sendMessage(const std::vector<ChatMessage>& messages, 
                                   const std::string& systemPrompt) {
    json requestBody;
    requestBody["model"] = model;
    requestBody["messages"] = json::array();
    
    // Add system prompt if provided
    if (!systemPrompt.empty()) {
        json systemMsg;
        systemMsg["role"] = "system";
        systemMsg["content"] = systemPrompt;
        requestBody["messages"].push_back(systemMsg);
    }
    
    // Add conversation messages
    for (const auto& msg : messages) {
        json message;
        message["role"] = msg.role;
        message["content"] = msg.content;
        requestBody["messages"].push_back(message);
    }
    
    requestBody["temperature"] = 0.7;
    requestBody["max_tokens"] = 1000;
    
    std::string requestStr = requestBody.dump();
    std::string response = makeRequest(requestStr);
    json responseJson = parseResponse(response);
    
    if (responseJson.contains("choices") && !responseJson["choices"].empty()) {
        return responseJson["choices"][0]["message"]["content"];
    }
    
    throw std::runtime_error("Invalid response format from API");
}

bool APIHandler::testConnection() {
    try {
        std::vector<ChatMessage> testMsg;
        testMsg.push_back({"user", "Hello"});
        std::string response = sendMessage(testMsg, "You are a helpful assistant.");
        return !response.empty();
    } catch (const std::exception& e) {
        std::cerr << "Connection test failed: " << e.what() << std::endl;
        return false;
    }
}