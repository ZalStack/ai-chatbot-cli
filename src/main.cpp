#include "chatbot.h"
#include "env_loader.h"  
#include <iostream>
#include <signal.h>
#include <cstdlib>

Chatbot* globalChatbot = nullptr;

void signalHandler(int signal) {
    std::cout << "\n\n🛑 Interrupt signal (" << signal << ") received.\n";
    if (globalChatbot) {
        globalChatbot->stop();
    }
    std::cout << "👋 Goodbye!\n";
    exit(0);
}

int main() {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    std::cout << "📁 Loading configuration from .env file..." << std::endl;
    if (!EnvLoader::load(".env")) {
        std::cerr << "⚠️  Warning: Could not load .env file." << std::endl;
        std::cerr << "Using system environment variables if available." << std::endl;
    }
    
    try {
        std::cout << "\n========================================\n";
        std::cout << "   AI Chatbot CLI v1.0\n";
        std::cout << "   Powered by Groq API & C++\n";
        std::cout << "========================================\n\n";
        
        Chatbot chatbot;
        globalChatbot = &chatbot;
        
        chatbot.run();
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Fatal error: " << e.what() << std::endl;
        return 1;
    }
}