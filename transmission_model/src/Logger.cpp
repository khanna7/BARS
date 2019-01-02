
#include "Logger.h"

namespace TransModel {

Logger *Logger::instance_ = nullptr;

Logger::Logger() : logs(SERO_LOG + 1, std::make_shared<NullLog>()) {}

Logger::~Logger() {
    for (auto log : logs) {
        log->close();
    }

    instance_ = nullptr;
}

Logger* Logger::instance() {
    if (instance_ == nullptr) {
       instance_ = new Logger();
    }
    return instance_;
}

void Logger::addLog(size_t log_id, std::string fname) {
    logs[log_id] = std::make_shared<FileLog>(fname);
}

std::shared_ptr<Log> Logger::getLog(size_t log_id) { 
    return logs[log_id]; 
}

FileLog::FileLog(std::string fname) : out(fname) {}

Log &FileLog::operator<<(const std::string &val)  {
    out << val;
    return *this;
}

Log &FileLog::operator<<(double val) {
    out << val;
    return *this;
}

Log &FileLog::operator<<(float val) {
    out << val;
    return *this;
}

Log &FileLog::operator<<(unsigned int val) {
    out << val;
    return *this;
}

Log &FileLog::operator<<(int val) {
    out << val;
    return *this;
}

void FileLog::close() {
     out.close(); 
}

} // namespace TransModel
