#ifndef HISTORY_H
#define HISTORY_H

#include <string>
#include <vector>
#include <memory>
#include "database.h"
#include "api_handler.h"

class History {
private:
    std::shared_ptr<Database> db;
    std::string sessionId;
    std::vector<ChatMessage> conversationHistory;
    
public:
    History(std::shared_ptr<Database> database);
    
    void setSession(const std::string& id);
    std::string getSession() const;
    void addMessage(const std::string& role, const std::string& content);
    std::vector<ChatMessage> getRecentMessages(int count = 10) const;
    std::vector<Message> loadFromDatabase(int limit = 50);
    bool clearHistory();
    std::vector<std::string> getAvailableSessions();
    void displayHistory() const;
    size_t getMessageCount() const;
};

#endif