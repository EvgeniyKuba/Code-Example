#ifndef C_LOG_H
#define C_LOG_H

#include <Windows.h>

namespace KGF
{
    //////////////////////////////////////////////////////////////////////////
    enum LogLevel
    {
        FATAL,
        FAIL,
        WARNING,
        INFO,
        DEBUG,
        SCRIPTERS       
    };
    //////////////////////////////////////////////////////////////////////////
    class CLog 
    {
    public:
        CLog();
        ~CLog() = default;

        void Write(LogLevel level, const std::wstring& message);

    private:

        std::string filepath = "data/settings/log.log";;

        // -- Получение уровня
        std::wstring levelToString(LogLevel level) const; 

        // -- Получение времени
        std::wstring getCurrentTime() const;

        void setUp();
        void logHeader();
        
    };

}

#endif
