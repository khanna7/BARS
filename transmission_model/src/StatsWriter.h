/*
 * StatsWriter.h
 *
 *  Created on: Mar 21, 2016
 *      Author: nick
 */

#ifndef SRC_STATSWRITER_H_
#define SRC_STATSWRITER_H_

#include <string>
#include <vector>
#include <iostream>

#include "FileOutput.h"

namespace TransModel {

// T is a struct that is accumulated by this
// class and then written out when a buffer size is
// reached
template <typename T>
class StatsWriterI {

public:
    StatsWriterI() {}
    virtual ~StatsWriterI() {}
    virtual void addOutput(const T& output) = 0;
};

// T is a struct that is accumulated by this
// class and then written out when a buffer size is
// reached.
template <typename T>
class StatsWriter : public StatsWriterI<T> {

private:
    FileOutput out;
    std::vector<T> data;
    unsigned int buffer_;

    void writeData();

public:
    StatsWriter(const std::string& fname, const std::string& header, unsigned int buffer);
    virtual ~StatsWriter();

    void addOutput(const T& output) override;
};

template<typename T>
StatsWriter<T>::StatsWriter(const std::string& fname, const std::string& header, unsigned int buffer) : StatsWriterI<T>{}, out{fname}, data{},
    buffer_{buffer} {
    out << header << "\n";
}


template<typename T>
void StatsWriter<T>::addOutput(const T& output) {
    data.push_back(output);
    if (data.size() >= buffer_) {
        writeData();
    }
}

template<typename T>
void StatsWriter<T>::writeData() {
    for (auto& item : data) {
        item.writeTo(out);
    }
    out.flush();
    data.clear();
}

template<typename T>
StatsWriter<T>::~StatsWriter() {
    writeData();
}

// T is a struct that is accumulated by this
// class and then written out when a buffer size is
// reached.
template <typename T>
class NullStatsWriter : public StatsWriterI<T> {

public:
    NullStatsWriter();
    virtual ~NullStatsWriter() {}

    void addOutput(const T& output) override {}
};

template <typename T>
NullStatsWriter<T>::NullStatsWriter() : StatsWriterI<T>() {}

} /* namespace TransModel */

#endif /* SRC_STATSWRITER_H_ */
