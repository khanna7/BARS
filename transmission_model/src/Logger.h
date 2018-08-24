#ifndef SRC_LOGGER_H
#define SRC_LOGGER_H

#include <memory>
#include <vector>
#include <string>

#include "FileOutput.h"


namespace TransModel {

// SERO_LOG should always be the max
const size_t BASE_LOG = 0;
const size_t NET_LOG = 1;
const size_t SERO_LOG = 2;

class Log {

public:

    Log() {}
    virtual ~Log() {}

    virtual Log& operator<<(const std::string& val) = 0;
    virtual Log& operator<<(double val) = 0;
    virtual Log& operator<<(float val) = 0;
    virtual Log& operator<<(unsigned int val) = 0;
    virtual Log& operator<<(int val) = 0;
    virtual void close() = 0;
};

class FileLog : public Log {

private:
    FileOutput out;


public:
    FileLog(std::string fname);
    Log& operator<<(const std::string& val) override;
    Log& operator<<(double val) override;
    Log& operator<<(float val) override;
    Log& operator<<(unsigned int val) override;
    Log& operator<<(int val) override;

    void close() override;

};

class NullLog : public Log {

public:
    Log& operator<<(const std::string& val) override {
        return *this;
    }
    Log& operator<<(double val) override {
        return *this;
    }
    Log& operator<<(float val) override  {
        return *this;
    }
    Log& operator<<(unsigned int val) override  {
        return *this;
    }
    Log& operator<<(int val) override  {
        return *this;
    }
   

    void close() override {}

};



class Logger {

private:
    std::vector<std::shared_ptr<Log>> logs;

    static Logger* instance_;

public:
    Logger();
    virtual ~Logger();

    static Logger* instance();
    void addLog(size_t log_id, std::string fname);
    std::shared_ptr<Log> getLog(size_t log_id);
};

}



#endif /* SRC_LOGGER_H */