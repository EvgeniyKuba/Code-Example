#include "CLog.h"

namespace KGF
{
    CLog::CLog()
    {
        setUp();
    }

    void CLog::setUp()
    {
        std::locale::global(std::locale("ru_RU.UTF-8"));
        std::wofstream ofs(filepath, std::ios::trunc);

        ofs.close();
        logHeader();
    }

	void CLog::Write(LogLevel level, const std::wstring& message)
	{
        std::wofstream ofs(filepath, std::ios::app);
        if (!ofs)
        {
            MessageBox(NULL, L"Лог-файл не обнаружен", L"Внимание!", MB_ICONEXCLAMATION);
            return;
        }

        ofs << getCurrentTime() << L" " << levelToString(level) << message << std::endl;
	}

    void CLog::logHeader()
    {
        std::wofstream ofs(filepath, std::ios::app);
        if (!ofs) return;

        ofs << L" " << std::endl;
        ofs << L"              ██╗  ██╗   ██████╗   ███████╗            " << std::endl;
        ofs << L"              ██║ ██╔╝  ██╔═══██╗  ██╔════╝            " << std::endl;
        ofs << L"              █████╔╝   ██║        ███████╗            " << std::endl;
        ofs << L"              ██╔═██╗   ██║  ██║   ██╔════╝            " << std::endl;
        ofs << L"              ██║  ██╗  ╚██████╔╝  ██║                 " << std::endl;
        ofs << L"              ╚═╝  ╚═╝   ╚═════╝   ╚═╝                 " << std::endl;
        ofs << L" " << std::endl;
        ofs << L"=============== Kuba Games Framework Log ================" << std::endl;
        ofs << L"               Старт: " << getCurrentTime() << std::endl;
        ofs << L"=========================================================" << std::endl;
        ofs << L" " << std::endl;
    }

    std::wstring CLog::levelToString(LogLevel level) const
    {
        switch (level) {
        case FATAL: return L"[FATAL] ";
        case FAIL: return L"[FAIL] ";
        case WARNING: return L"[WARNING] ";
        case INFO: return L"[INFO] ";
        case DEBUG: return L"[DEBUG] ";
        case SCRIPTERS: return L"[SCRIPTERS] ";
        default: return L"";
        }
    }

    std::wstring CLog::getCurrentTime() const {
        std::wstringstream ss;
        std::time_t t = std::time(nullptr);
        std::tm tm{};
        localtime_s(&tm, &t);  // -- Получение локального времени

        wchar_t buffer[100];
        size_t bufferSize = sizeof(buffer) / sizeof(wchar_t);
        wcsftime(buffer, bufferSize, L"%Y-%m-%d %H:%M:%S", &tm);
        ss << buffer;
        return ss.str();
    }

}