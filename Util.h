#pragma once
#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>

class Util {
public:

    class NoFileException : public std::exception {
    public:
        virtual const char * what() {
            return "File not found.";
        }
    };

    //caller's job to delete content
    static char* slurp(const char* fn, const char* mode="rt");
};

class Log {

public:
    enum LogLevel {DEBUG,INFO, WARNING, ERR};
    Log(LogLevel aLevel) : pCurrentLevel(aLevel) {
    };
    
    //When Log() is called anonymously, will be called at end of that line.
    ~Log() {
        if (pCurrentLevel >= pLevel)
            *pFile << pStrStream.str() << std::endl;
    }

    static bool configure(std::string aFilename, LogLevel aLevel) {
        pLevel = aLevel;
        pFile = new std::fstream( aFilename.c_str(), 
            std::ios_base::out | std::ios_base::app | std::ios_base::ate );

        if (!pFile->fail())
            *pFile << "Logging initialized." << std::endl;
        return !pFile->fail();
    }

    std::stringstream& log() { return pStrStream; }
    
    static const char* levelToString(LogLevel aLevel) {
        if (aLevel == DEBUG)
            return "DEBUG";
        if (aLevel == INFO)
            return "INFO";
        if (aLevel == WARNING) 
            return "WARNING";
        if (aLevel == ERR) 
            return "ERROR";
        return "(INVALID LogLevel)";
    }

private:
    static std::fstream* pFile;
    static LogLevel pLevel;
    LogLevel pCurrentLevel;
    std::stringstream pStrStream;
};

#define LOG(level) Log(level).log() << "[" << Log::levelToString(level) << " (" << __FILE__ << ": " << __LINE__ << ")] "
