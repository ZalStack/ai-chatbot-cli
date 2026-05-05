#include "history.h"
#include "utils.h"
#include <iostream>
#include <algorithm>

History::History(std::shared_ptr<Database> database) 
    : db(database), sessionId(Utils::generateSessionId()) {
}

void History::setSession(const std::string& id) {
    sessionId = id;
    conversationHistory.clear();
    // Load messages from database for this session
    auto dbMessages = db->getConversationHistory(sessionId, 50);
    for (const auto& msg : dbMessages) {
        conversationHistory.push_back({msg.role, msg.content});
    }
}

std::string History::getSession() const {
    return sessionId;
}

void History::addMessage(const std::string& role, const std::string& content) {
    conversationHistory.push_back({role, content});
    // Keep only last 50 messages in memory
    if (conversationHistory.size() > 50) {
        conversationHistory.erase(conversationHistory.begin());
    }
    // Save to database
    db->saveMessage(sessionId, role, content);
}

std::vector<ChatMessage> History::getRecentMessages(int count) const {
    if (conversationHistory.empty()) {
        return {};
    }
    
    int startIdx = std::max(0, static_cast<int>(conversationHistory.size()) - count);
    return std::vector<ChatMessage>(conversationHistory.begin() + startIdx, 
                                   conversationHistory.end());
}

std::vector<Message> History::loadFromDatabase(int limit) {
    return db->getConversationHistory(sessionId, limit);
}

bool History::clearHistory() {
    conversationHistory.clear();
    return db->clearHistory(sessionId);
}

std::vector<std::string> History::getAvailableSessions() {
    return db->getSessions();
}

void History::displayHistory() const {
    auto messages = db->getConversationHistory(sessionId, 20);
    
    if (messages.empty()) {
        std::cout << "No conversation history found." << std::endl;
        return;
    }
    
    for (const auto& msg : messages) {
        std::string roleColor = (msg.role == "user") ? "green" : "cyan";
        std::cout << "\n[" << msg.timestamp << "] ";
        Utils::printColored(msg.role + ": ", roleColor);
        std::cout << msg.content << std::endl;
    }
}

size_t History::getMessageCount() const {
    return conversationHistory.size();
}