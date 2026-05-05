#ifndef DATABASE_H
#define DATABASE_H

#include <mysql/mysql.h>
#include <string>
#include <vector>
#include <memory>
#include <mutex>

struct Message {
    int id;
    std::string session_id;
    std::string role;
    std::string content;
    std::string timestamp;
};

class Database {
private:
    MYSQL* conn;
    std::string host;
    std::string user;
    std::string password;
    std::string dbname;
    std::mutex db_mutex;
    
    bool connect();
    void disconnect();

public:
    Database(const std::string& host, const std::string& user, 
             const std::string& password, const std::string& dbname);
    ~Database();
    
    bool initialize();
    bool saveMessage(const std::string& sessionId, const std::string& role, 
                    const std::string& content);
    std::vector<Message> getConversationHistory(const std::string& sessionId, int limit = 50);
    bool clearHistory(const std::string& sessionId);
    std::vector<std::string> getSessions();
    bool saveCustomPrompt(const std::string& name, const std::string& prompt);
    std::string getActivePrompt();
    std::vector<std::pair<std::string, std::string>> getAllPrompts();
    bool setActivePrompt(const std::string& name);
};

#endif