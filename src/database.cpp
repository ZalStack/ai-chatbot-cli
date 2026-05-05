#include "database.h"
#include "utils.h"
#include <stdexcept>
#include <algorithm>
#include <cstring>

Database::Database(const std::string& host, const std::string& user, 
                   const std::string& password, const std::string& dbname)
    : conn(nullptr), host(host), user(user), password(password), dbname(dbname) {
}

Database::~Database() {
    disconnect();
}

bool Database::connect() {
    std::lock_guard<std::mutex> lock(db_mutex);
    
    conn = mysql_init(nullptr);
    if (!conn) {
        std::cerr << "MySQL/MariaDB initialization failed" << std::endl;
        return false;
    }
    
    // MySQL connection
    if (!mysql_real_connect(conn, host.c_str(), user.c_str(), 
                           password.c_str(), dbname.c_str(), 3306, nullptr, 0)) {
        std::cerr << "Database connection failed: " << mysql_error(conn) << std::endl;
        return false;
    }
    
    return true;
}

void Database::disconnect() {
    if (conn) {
        mysql_close(conn);
        conn = nullptr;
    }
}

bool Database::initialize() {
    if (!connect()) {
        return false;
    }
    
    const char* createTableSQL = 
        "CREATE TABLE IF NOT EXISTS conversations ("
        "id INT AUTO_INCREMENT PRIMARY KEY, "
        "session_id VARCHAR(255) NOT NULL, "
        "role ENUM('user', 'assistant', 'system') NOT NULL, "
        "content TEXT NOT NULL, "
        "timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
        "INDEX idx_session (session_id)"
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci";
    
    if (mysql_query(conn, createTableSQL)) {
        std::cerr << "Failed to create conversations table: " << mysql_error(conn) << std::endl;
        return false;
    }
    
    const char* createPromptsTableSQL = 
        "CREATE TABLE IF NOT EXISTS custom_prompts ("
        "id INT AUTO_INCREMENT PRIMARY KEY, "
        "name VARCHAR(255) NOT NULL UNIQUE, "
        "prompt_text TEXT NOT NULL, "
        "is_active BOOLEAN DEFAULT FALSE, "
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci";
    
    if (mysql_query(conn, createPromptsTableSQL)) {
        std::cerr << "Failed to create prompts table: " << mysql_error(conn) << std::endl;
        return false;
    }
    
    return true;
}

bool Database::saveMessage(const std::string& sessionId, const std::string& role, 
                          const std::string& content) {
    std::lock_guard<std::mutex> lock(db_mutex);
    
    if (!conn || mysql_ping(conn)) {
        if (!connect()) return false;
    }
    
    // PERBAIKI: Escape string yang benar dengan buffer yang cukup besar
    size_t contentLen = content.length();
    size_t escapedLen = (contentLen * 2) + 1;
    char* escapedContent = new char[escapedLen];
    
    mysql_real_escape_string(conn, escapedContent, content.c_str(), contentLen);
    
    // Escape session_id juga
    size_t sessionLen = sessionId.length();
    size_t escapedSessionLen = (sessionLen * 2) + 1;
    char* escapedSession = new char[escapedSessionLen];
    mysql_real_escape_string(conn, escapedSession, sessionId.c_str(), sessionLen);
    
    // Escape role juga  
    size_t roleLen = role.length();
    size_t escapedRoleLen = (roleLen * 2) + 1;
    char* escapedRole = new char[escapedRoleLen];
    mysql_real_escape_string(conn, escapedRole, role.c_str(), roleLen);
    
    // Buat query menggunakan string yang sudah di-escape
    std::string query = "INSERT INTO conversations (session_id, role, content) VALUES ('";
    query += escapedSession;
    query += "', '";
    query += escapedRole;
    query += "', '";
    query += escapedContent;
    query += "')";
    
    // Debug: print query (optional)
    // std::cout << "DEBUG SQL: " << query << std::endl;
    
    int result = mysql_query(conn, query.c_str());
    
    // Bersihkan memory
    delete[] escapedContent;
    delete[] escapedSession;
    delete[] escapedRole;
    
    if (result != 0) {
        std::cerr << "Failed to save message: " << mysql_error(conn) << std::endl;
        std::cerr << "Query: " << query.substr(0, 100) << "..." << std::endl;
        return false;
    }
    
    return true;
}

std::vector<Message> Database::getConversationHistory(const std::string& sessionId, int limit) {
    std::lock_guard<std::mutex> lock(db_mutex);
    std::vector<Message> messages;
    
    if (!conn || mysql_ping(conn)) {
        if (!connect()) return messages;
    }
    
    // Escape session_id
    size_t sessionLen = sessionId.length();
    char* escapedSession = new char[(sessionLen * 2) + 1];
    mysql_real_escape_string(conn, escapedSession, sessionId.c_str(), sessionLen);
    
    std::string query = "SELECT id, session_id, role, content, timestamp "
                       "FROM conversations WHERE session_id = '";
    query += escapedSession;
    query += "' ORDER BY timestamp DESC LIMIT ";
    query += std::to_string(limit);
    
    delete[] escapedSession;
    
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Failed to get history: " << mysql_error(conn) << std::endl;
        return messages;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) return messages;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        Message msg;
        msg.id = row[0] ? std::stoi(row[0]) : 0;
        msg.session_id = row[1] ? row[1] : "";
        msg.role = row[2] ? row[2] : "";
        msg.content = row[3] ? row[3] : "";
        msg.timestamp = row[4] ? row[4] : "";
        messages.push_back(msg);
    }
    
    mysql_free_result(result);
    
    // Reverse to get chronological order
    std::reverse(messages.begin(), messages.end());
    
    return messages;
}

bool Database::clearHistory(const std::string& sessionId) {
    std::lock_guard<std::mutex> lock(db_mutex);
    
    if (!conn || mysql_ping(conn)) {
        if (!connect()) return false;
    }
    
    // Escape session_id
    size_t sessionLen = sessionId.length();
    char* escapedSession = new char[(sessionLen * 2) + 1];
    mysql_real_escape_string(conn, escapedSession, sessionId.c_str(), sessionLen);
    
    std::string query = "DELETE FROM conversations WHERE session_id = '";
    query += escapedSession;
    query += "'";
    
    delete[] escapedSession;
    
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Failed to clear history: " << mysql_error(conn) << std::endl;
        return false;
    }
    
    return true;
}

std::vector<std::string> Database::getSessions() {
    std::lock_guard<std::mutex> lock(db_mutex);
    std::vector<std::string> sessions;
    
    if (!conn || mysql_ping(conn)) {
        if (!connect()) return sessions;
    }
    
    const char* query = "SELECT DISTINCT session_id, MAX(timestamp) as max_time "
                       "FROM conversations GROUP BY session_id ORDER BY max_time DESC";
    
    if (mysql_query(conn, query)) {
        return sessions;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) return sessions;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        if (row[0]) {
            sessions.push_back(row[0]);
        }
    }
    
    mysql_free_result(result);
    return sessions;
}

bool Database::saveCustomPrompt(const std::string& name, const std::string& prompt) {
    std::lock_guard<std::mutex> lock(db_mutex);
    
    if (!conn || mysql_ping(conn)) {
        if (!connect()) return false;
    }
    
    // Escape strings
    size_t nameLen = name.length();
    char* escapedName = new char[(nameLen * 2) + 1];
    mysql_real_escape_string(conn, escapedName, name.c_str(), nameLen);
    
    size_t promptLen = prompt.length();
    char* escapedPrompt = new char[(promptLen * 2) + 1];
    mysql_real_escape_string(conn, escapedPrompt, prompt.c_str(), promptLen);
    
    std::string query = "INSERT INTO custom_prompts (name, prompt_text) VALUES ('";
    query += escapedName;
    query += "', '";
    query += escapedPrompt;
    query += "') ON DUPLICATE KEY UPDATE prompt_text = '";
    query += escapedPrompt;
    query += "'";
    
    int result = mysql_query(conn, query.c_str());
    
    delete[] escapedName;
    delete[] escapedPrompt;
    
    return result == 0;
}

std::string Database::getActivePrompt() {
    std::lock_guard<std::mutex> lock(db_mutex);
    
    if (!conn || mysql_ping(conn)) {
        if (!connect()) return "You are a helpful AI assistant.";
    }
    
    const char* query = "SELECT prompt_text FROM custom_prompts WHERE is_active = TRUE LIMIT 1";
    
    if (mysql_query(conn, query)) {
        return "You are a helpful AI assistant.";
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) return "You are a helpful AI assistant.";
    
    MYSQL_ROW row = mysql_fetch_row(result);
    std::string prompt = (row && row[0]) ? row[0] : "You are a helpful AI assistant.";
    
    mysql_free_result(result);
    return prompt;
}

std::vector<std::pair<std::string, std::string>> Database::getAllPrompts() {
    std::lock_guard<std::mutex> lock(db_mutex);
    std::vector<std::pair<std::string, std::string>> prompts;
    
    if (!conn || mysql_ping(conn)) {
        if (!connect()) return prompts;
    }
    
    const char* query = "SELECT name, prompt_text FROM custom_prompts";
    
    if (mysql_query(conn, query)) {
        return prompts;
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) return prompts;
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        if (row[0] && row[1]) {
            prompts.emplace_back(row[0], row[1]);
        }
    }
    
    mysql_free_result(result);
    return prompts;
}

bool Database::setActivePrompt(const std::string& name) {
    std::lock_guard<std::mutex> lock(db_mutex);
    
    if (!conn || mysql_ping(conn)) {
        if (!connect()) return false;
    }
    
    // Reset all prompts
    mysql_query(conn, "UPDATE custom_prompts SET is_active = FALSE");
    
    // Set the specified prompt as active
    size_t nameLen = name.length();
    char* escapedName = new char[(nameLen * 2) + 1];
    mysql_real_escape_string(conn, escapedName, name.c_str(), nameLen);
    
    std::string query = "UPDATE custom_prompts SET is_active = TRUE WHERE name = '";
    query += escapedName;
    query += "'";
    
    int result = mysql_query(conn, query.c_str());
    
    delete[] escapedName;
    
    return result == 0;
}