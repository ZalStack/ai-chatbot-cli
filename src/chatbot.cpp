#include "chatbot.h"
#include <iostream>
#include <thread>
#include <chrono>

Chatbot::Chatbot() : running(false) {
    // Load environment variables
    std::string dbHost = Utils::getEnv("DB_HOST", "localhost");
    std::string dbUser = Utils::getEnv("DB_USER", "root");
    std::string dbPassword = Utils::getEnv("DB_PASSWORD", "");
    std::string dbName = Utils::getEnv("DB_NAME", "chatbot_db");
    
    std::string apiKey = Utils::getEnv("GROQ_API_KEY", "");
    std::string apiUrl = Utils::getEnv("GROQ_API_URL", "https://api.groq.com/openai/v1/chat/completions");
    std::string model = Utils::getEnv("GROQ_MODEL", "llama-3.3-70b-versatile");
    
    // Initialize components
    db = std::make_shared<Database>(dbHost, dbUser, dbPassword, dbName);
    history = std::make_unique<History>(db);
    
    api = std::make_unique<APIHandler>(apiKey, apiUrl, model);
    
    // Default system prompt
    systemPrompt = "You are a helpful AI assistant.";
}

Chatbot::~Chatbot() {
    stop();
}

bool Chatbot::initialize() {
    std::cout << "Initializing AI Chatbot CLI..." << std::endl;
    
    // Initialize database
    if (!db->initialize()) {
        std::cerr << "Failed to initialize database. Running without database support." << std::endl;
        // Continue without database - graceful degradation
    }
    
    // Load active prompt from database
    try {
        std::string dbPrompt = db->getActivePrompt();
        if (!dbPrompt.empty()) {
            systemPrompt = dbPrompt;
        }
    } catch (...) {
        // Use default prompt
    }
    
    // Test API connection
    std::cout << "Testing API connection... ";
    if (api->testConnection()) {
        std::cout << "Connected!" << std::endl;
    } else {
        std::cerr << "Failed to connect to API. Check your API key and internet connection." << std::endl;
        return false;
    }
    
    return true;
}

void Chatbot::displayWelcome() {
    Utils::clearScreen();
    Utils::printColored("\n╔══════════════════════════════════════════╗\n", "cyan");
    Utils::printColored("║        AI Chatbot CLI - Powered by Groq   ║\n", "cyan");
    Utils::printColored("║        Model: Llama 3.3 70B Versatile     ║\n", "cyan");
    Utils::printColored("╚══════════════════════════════════════════╝\n\n", "cyan");
    
    std::cout << "Session ID: " << history->getSession() << std::endl;
    std::cout << "Type '/help' for available commands\n" << std::endl;
}

void Chatbot::displayHelp() {
    std::cout << "\n📋 Available Commands:\n";
    std::cout << "────────────────────────────────────────\n";
    std::cout << "/help          - Show this help message\n";
    std::cout << "/history       - Show conversation history\n";
    std::cout << "/clear         - Clear conversation history\n";
    std::cout << "/prompts       - List available system prompts\n";
    std::cout << "/prompt <name> - Switch to a specific prompt\n";
    std::cout << "/save <name> <text> - Save custom prompt\n";
    std::cout << "/sessions      - List available sessions\n";
    std::cout << "/switch <id>   - Switch to a different session\n";
    std::cout << "/system        - Show current system prompt\n";
    std::cout << "/export        - Export conversation to file\n";
    std::cout << "/quit or /exit - Exit the chatbot\n";
    std::cout << "────────────────────────────────────────\n\n";
}

void Chatbot::processCommand(const std::string& input) {
    std::string cmd = Utils::trim(input);
    
    if (cmd == "/help") {
        displayHelp();
    }
    else if (cmd == "/history") {
        std::cout << "\n📜 Conversation History:\n";
        history->displayHistory();
    }
    else if (cmd == "/clear") {
        if (history->clearHistory()) {
            std::cout << "✅ History cleared successfully.\n";
        }
    }
    else if (cmd == "/prompts") {
        auto prompts = db->getAllPrompts();
        std::cout << "\n📝 Available System Prompts:\n";
        for (const auto& prompt : prompts) {
            std::cout << "  • " << prompt.first << (prompt.first == "default" ? " (active)" : "") << "\n";
        }
    }
    else if (cmd.substr(0, 8) == "/prompt ") {
        std::string promptName = Utils::trim(cmd.substr(8));
        if (db->setActivePrompt(promptName)) {
            systemPrompt = db->getActivePrompt();
            std::cout << "✅ Switched to prompt: " << promptName << "\n";
        } else {
            std::cout << "❌ Failed to switch prompt.\n";
        }
    }
    else if (cmd.substr(0, 6) == "/save ") {
        size_t spacePos = cmd.find(' ', 6);
        if (spacePos != std::string::npos) {
            std::string name = cmd.substr(6, spacePos - 6);
            std::string text = cmd.substr(spacePos + 1);
            if (db->saveCustomPrompt(name, text)) {
                std::cout << "✅ Prompt saved successfully.\n";
            }
        }
    }
    else if (cmd == "/sessions") {
        auto sessions = history->getAvailableSessions();
        std::cout << "\n📂 Available Sessions:\n";
        for (const auto& session : sessions) {
            std::cout << "  • " << session << "\n";
        }
    }
    else if (cmd.substr(0, 8) == "/switch ") {
        std::string sessionId = Utils::trim(cmd.substr(8));
        history->setSession(sessionId);
        std::cout << "✅ Switched to session: " << sessionId << "\n";
    }
    else if (cmd == "/system") {
        std::cout << "\n🤖 Current System Prompt:\n";
        Utils::printColored(systemPrompt + "\n", "yellow");
    }
    else if (cmd == "/export") {
        std::string filename = "chat_export_" + history->getSession() + ".txt";
        std::ofstream file(filename);
        auto messages = history->loadFromDatabase();
        for (const auto& msg : messages) {
            file << "[" << msg.timestamp << "] " << msg.role << ": " << msg.content << "\n";
        }
        file.close();
        std::cout << "✅ Conversation exported to " << filename << "\n";
    }
    else {
        std::cout << "❌ Unknown command. Type /help for available commands.\n";
    }
}

void Chatbot::handleUserInput(const std::string& input) {
    if (input.empty()) return;
    
    // Add user message to history
    history->addMessage("user", input);
    
    // Show typing indicator
    std::cout << "\n🤖 ";
    Utils::printColored("AI is thinking...", "yellow");
    std::cout.flush();
    
    try {
        // Get recent messages for context
        auto recentMessages = history->getRecentMessages(10);
        
        // Send to API
        std::string response = api->sendMessage(recentMessages, systemPrompt);
        
        // Clear typing indicator
        std::cout << "\r";
        
        // Display response
        Utils::printColored("\n🤖 AI: ", "cyan");
        std::cout << response << std::endl;
        
        // Save AI response to history
        history->addMessage("assistant", response);
        
    } catch (const std::exception& e) {
        std::cout << "\r❌ Error: " << e.what() << std::endl;
    }
}

void Chatbot::run() {
    if (!initialize()) {
        std::cerr << "Failed to initialize chatbot. Exiting." << std::endl;
        return;
    }
    
    running = true;
    displayWelcome();
    
    std::string input;
    while (running) {
        Utils::printColored("\n👤 You: ", "green");
        std::getline(std::cin, input);
        
        input = Utils::trim(input);
        
        if (input.empty()) continue;
        
        // Check for exit commands
        if (input == "/quit" || input == "/exit") {
            std::cout << "\n👋 Goodbye! Thanks for using AI Chatbot CLI.\n";
            break;
        }
        
        // Process commands (starting with /)
        if (input[0] == '/') {
            processCommand(input);
        } else {
            handleUserInput(input);
        }
    }
    
    running = false;
}

void Chatbot::stop() {
    running = false;
}