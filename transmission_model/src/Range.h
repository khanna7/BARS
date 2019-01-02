#ifndef SRC_RANGE_H_
#define SRC_RANGE_H_

#include <string>
#include <iostream>

namespace TransModel {

/**
 * Range from [x, y)
 */
template<typename T>
class Range {

private:
    T start_, end_;

public:
    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, Range<U>& range);
    Range(T start, T end);
    virtual ~Range();
    bool within(T val);
};

template<typename T>
Range<T>::Range(T start, T end) :
        start_(start), end_(end) {
}

template<typename T>
Range<T>::~Range() {
}

template<typename T>
bool Range<T>::within(T val) {
    return val >= start_ && val < end_;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, Range<T>& range) {
    return os << "TransModel::Range(" << range.start_ << ", " << range.end_ << ")";
}


Range<double> parse_range(const std::string& range_string);

}


#endif /* SRC_RANGE_H_ */
