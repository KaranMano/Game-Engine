#include <SDL3/SDL.h>

#include <cstdio>
#include <mutex>

#include <logging/log.hpp>

static bool g_logToFile = false;
static std::string g_logFilename;
static std::mutex g_logMutex;

static const char* GetPrefix(LogLevel level) {
    switch (level) {
    case LogLevel::Trace:    return "[TRACE] ";
    case LogLevel::Debug:    return "[DEBUG] ";
    case LogLevel::Info:     return "[INFO ] ";
    case LogLevel::Warn:     return "[WARN ] ";
    case LogLevel::Error:    return "[ERROR] ";
    case LogLevel::Critical: return "[CRIT ] ";
    default:                 return "";
    }
}

static const char* GetTimeStamp() {
    static char buffer[32];
    const auto now = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&time));
    return buffer;
}

void LogToFile(const std::string& filename, bool enable) {
    std::lock_guard<std::mutex> lock(g_logMutex);
    g_logToFile = enable;
    g_logFilename = filename;
}

void Log(LogLevel level, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    // Format the message
    char buf[1024];
    SDL_vsnprintf(buf, sizeof(buf), fmt, args);

    // Use SDL for console logging
    SDL_LogPriority sdlPriority = SDL_LOG_PRIORITY_INFO;
    switch (level) {
        case LogLevel::Warn:     sdlPriority = SDL_LOG_PRIORITY_WARN; break;
        case LogLevel::Error:    sdlPriority = SDL_LOG_PRIORITY_ERROR; break;
        case LogLevel::Critical: sdlPriority = SDL_LOG_PRIORITY_CRITICAL; break;
        case LogLevel::Debug:    sdlPriority = SDL_LOG_PRIORITY_DEBUG; break;
        case LogLevel::Trace:    sdlPriority = SDL_LOG_PRIORITY_VERBOSE; break;
        default:                 break;
    }

    SDL_LogMessage(
        SDL_LOG_CATEGORY_APPLICATION,
        sdlPriority,
        "[%s] %s%s",
        GetTimeStamp(),
        GetPrefix(level),
        buf
    );

    // write to file if enabled
    if (g_logToFile && !g_logFilename.empty()) {
        std::lock_guard<std::mutex> lock(g_logMutex);
        FILE* fp = fopen(g_logFilename.c_str(), "a");
        if (fp) {
            fprintf(fp, "[%s] %s%s\n", GetTimeStamp(), GetPrefix(level), buf);
            fclose(fp);
        }
        else {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open log file: %s", g_logFilename.c_str());
        }
    }

    va_end(args);
}