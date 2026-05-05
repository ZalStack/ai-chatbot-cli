#ifndef CHATBOT_H
#define CHATBOT_H

#include <string>
#include <vector>
#include <memory>
#include "api_handler.h"
#include "database.h"
#include "history.h"
#include "utils.h"

class Chatbot {
private:
    std::unique_ptr<APIHandler> api;
    std::shared_ptr<Database> db;
    std::unique_ptr<History> history;
    std::string systemPrompt;
    bool running;
    
    void processCommand(const std::string& input);
    void displayHelp();
    void displayWelcome();
    void handleUserInput(const std::string& input);
    
public:
    Chatbot();
    ~Chatbot();
    
    bool initialize();
    void run();
    void stop();
};

#endif