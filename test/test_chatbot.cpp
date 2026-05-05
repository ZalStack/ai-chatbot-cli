#include <iostream>
#include <cassert>
#include "utils.h"
#include "api_handler.h"

void testUtils() {
    std::cout << "Testing Utils..." << std::endl;
    
    // Test trim
    assert(Utils::trim("  hello  ") == "hello");
    assert(Utils::trim("test") == "test");
    
    // Test split
    auto tokens = Utils::split("hello,world,test", ',');
    assert(tokens.size() == 3);
    assert(tokens[0] == "hello");
    
    // Test toLowerCase
    assert(Utils::toLowerCase("HELLO") == "hello");
    
    // Test generateSessionId
    std::string sessionId = Utils::generateSessionId();
    assert(!sessionId.empty());
    assert(sessionId.find("session_") == 0);
    
    std::cout << "Utils tests passed!" << std::endl;
}

void testEnvVariables() {
    std::cout << "Testing Environment Variables..." << std::endl;
    
    std::string apiKey = Utils::getEnv("GROQ_API_KEY", "");
    std::string model = Utils::getEnv("GROQ_MODEL", "");
    
    assert(!apiKey.empty());
    assert(!model.empty());
    
    std::cout << "Environment variables tests passed!" << std::endl;
}

int main() {
    std::cout << "Running tests for AI Chatbot CLI\n";
    std::cout << "================================\n\n";
    
    try {
        testUtils();
        testEnvVariables();
        
        std::cout << "\n✅ All tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Test failed: " << e.what() << std::endl;
        return 1;
    }
}