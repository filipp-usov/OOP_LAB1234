#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <sstream>
#include <ctime>
#include <algorithm>

struct LogEvent {
    std::string timestamp;
    std::string level;
    std::string source;
    std::string message;

    LogEvent(std::string lvl, std::string src, std::string msg) : level(lvl), source(src), message(msg) {
        timestamp = getCurrentTimestamp();
    }

    static std::string getCurrentTimestamp() {
        time_t now = time(0);
        char buf[80];
        struct tm tstruct;
        localtime_s(&tstruct, &now);
        strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", &tstruct);
        return buf;
    }
};

class ILogFormatter {
public:
    virtual ~ILogFormatter() = default;
    virtual std::string format(const LogEvent& event) = 0;
};

class TextFormatter : public ILogFormatter {
public:
    std::string format(const LogEvent& e) override {
        return "[" + e.timestamp + "] [" + e.level + "] [" + e.source + "] " + e.message;
    }
};

class JsonFormatter : public ILogFormatter {
    std::string escape(const std::string& s) {
        std::string res;
        for (char c : s) {
            if (c == '"') res += "\\\"";
            else if (c == '\\') res += "\\\\";
            else if (c == '\n') res += "\\n";
            else res += c;
        }
        return res;
    }
public:
    std::string format(const LogEvent& e) override {
        return "{\"timestamp\": \"" + escape(e.timestamp) + "\", \"level\": \"" + escape(e.level) +
            "\", \"source\": \"" + escape(e.source) + "\", \"message\": \"" + escape(e.message) + "\"}";
    }
};

class CsvFormatter : public ILogFormatter {
    std::string escape(const std::string& s) {
        std::string res = s;
        std::replace(res.begin(), res.end(), ';', ',');
        std::replace(res.begin(), res.end(), '\n', ' ');
        return res;
    }
public:
    std::string format(const LogEvent& e) override {
        return e.timestamp + ";" + e.level + ";" + e.source + ";" + escape(e.message);
    }
};

class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const LogEvent& event) = 0;
    virtual void setFormatter(std::shared_ptr<ILogFormatter> formatter) = 0;
};

class ConsoleLogger : public ILogger {
protected:
    std::shared_ptr<ILogFormatter> formatter;
public:
    ConsoleLogger(std::shared_ptr<ILogFormatter> f) : formatter(f) {}
    void setFormatter(std::shared_ptr<ILogFormatter> f) override { formatter = f; }
    void log(const LogEvent& e) override { std::cout << formatter->format(e) << "\n"; }
};

class FileLogger : public ILogger {
private:
    std::ofstream file;
    std::shared_ptr<ILogFormatter> formatter;
public:
    FileLogger(const std::string& filename, std::shared_ptr<ILogFormatter> f) : formatter(f) {
        file.open(filename, std::ios::app);
    }
    ~FileLogger() override { if (file.is_open()) file.close(); }
    void setFormatter(std::shared_ptr<ILogFormatter> f) override { formatter = f; }
    void log(const LogEvent& e) override { if (file.is_open()) file << formatter->format(e) << "\n"; }
};

class NullLogger : public ILogger {
public:
    void setFormatter(std::shared_ptr<ILogFormatter>) override {}
    void log(const LogEvent&) override {}
};

class LoggingProxy : public ILogger {
private:
    std::unique_ptr<ILogger> currentLogger_;
    std::shared_ptr<ILogFormatter> formatter_;
public:
    LoggingProxy() : formatter_(std::make_shared<TextFormatter>()) {
        currentLogger_ = std::make_unique<ConsoleLogger>(formatter_);
    }

    void setLogger(std::unique_ptr<ILogger> logger) {
        currentLogger_ = std::move(logger);
        if (currentLogger_) currentLogger_->setFormatter(formatter_);
    }

    void setLoggerFromString(const std::string& type) {
        if (type == "console") setLogger(std::make_unique<ConsoleLogger>(formatter_));
        else if (type == "file") setLogger(std::make_unique<FileLogger>("game_log.txt", formatter_));
        else if (type == "null") setLogger(std::make_unique<NullLogger>());
    }

    void setFormatter(std::shared_ptr<ILogFormatter> formatter) override {
        formatter_ = formatter;
        if (currentLogger_) currentLogger_->setFormatter(formatter_);
    }

    void setFormatterFromString(const std::string& type) {
        if (type == "json") setFormatter(std::make_shared<JsonFormatter>());
        else if (type == "csv") setFormatter(std::make_shared<CsvFormatter>());
        else setFormatter(std::make_shared<TextFormatter>());
    }

    void log(const LogEvent& event) override {
        if (currentLogger_) currentLogger_->log(event);
    }
};

inline LoggingProxy& Logger() {
    static LoggingProxy instance;
    return instance;
}