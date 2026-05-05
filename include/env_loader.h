#ifndef ENV_LOADER_H
#define ENV_LOADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <algorithm>

#ifdef _WIN32
    #include <windows.h>
#endif

class EnvLoader {
public:
    static bool load(const std::string& filename = ".env") {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Warning: Cannot open .env file. Using system environment variables." << std::endl;
            return false;
        }
        
        std::string line;
        int loadedCount = 0;
        
        while (std::getline(file, line)) {
            // Skip empty lines and comments
            line = trim(line);
            if (line.empty() || line[0] == '#') {
                continue;
            }
            
            // Parse KEY=VALUE
            size_t equalPos = line.find('=');
            if (equalPos == std::string::npos) {
                continue;
            }
            
            std::string key = trim(line.substr(0, equalPos));
            std::string value = trim(line.substr(equalPos + 1));
            
            // Remove quotes if present
            if (value.size() >= 2) {
                if ((value.front() == '"' && value.back() == '"') ||
                    (value.front() == '\'' && value.back() == '\'')) {
                    value = value.substr(1, value.size() - 2);
                }
            }
            
            // Set environment variable
#ifdef _WIN32
            SetEnvironmentVariableA(key.c_str(), value.c_str());
#else
            setenv(key.c_str(), value.c_str(), 1);
#endif
            loadedCount++;
        }
        
        file.close();
        std::cout << "✅ Loaded " << loadedCount << " variables from " << filename << std::endl;
        return loadedCount > 0;
    }
    
private:
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
    }
};

#endif