#pragma once

#include <string>
#include <fstream>

class Logging
{
public:
    static Logging& GetInstance(const std::string& filename);
    void Log(const char* format, ...);
    void LogLine(const char* format, ...);
    void LogToAll(const char* format, ...);
    ~Logging();

private:
    Logging(const std::string& filename);
    Logging(const Logging&) = delete;
    Logging& operator=(const Logging&) = delete;

    void LogToConsole(const std::string& message);
    void LogToFile(const std::string& message);
    void LogTimestamp();

    std::ofstream logFile;
};
