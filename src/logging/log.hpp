#pragma once
#include <string>
#include <cstdarg>

enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Critical
};

/**
 * @brief Enable or disable logging to a file
 * @param filename Path to the log file
 * @param enable True to enable file logging, False to disable
 */
void LogToFile(const std::string& filename, bool enable);

/**
 * @brief Log a formatted message with the specified level
 * @param level Severity level of the message
 * @param fmt Printf-style format string
 * @param ... Variable arguments for format string
 */
void Log(LogLevel level, const char* fmt, ...);
