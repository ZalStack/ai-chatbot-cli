#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <iostream>

class Utils {
public:
    static std::string getCurrentTimestamp();
    static std::string generateSessionId();
    static std::string trim(const std::string& str);
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::string getEnv(const std::string& key, const std::string& defaultValue = "");
    static bool fileExists(const std::string& filename);
    static std::string readFile(const std::string& filename);
    static std::string toLowerCase(const std::string& str);
    static void clearScreen();
    static void printColored(const std::string& text, const std::string& color);
};

#endif