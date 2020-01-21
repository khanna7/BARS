#ifndef SRC_JAILINFECTIONRATECALC_H_
#define SRC_JAILINFECTIONRATECALC_H_

#include <list>

namespace TransModel {

class Serializer;

class JailInfRateCalculator  {

private:
    friend class Serializer;
    std::list<double> rates;
    unsigned int window_size_;
    double multiplier_;
    double default_rate_;

public:
    JailInfRateCalculator(unsigned int window_size, double multiplier, double default_rate);
    ~JailInfRateCalculator();
    void addInfectionRate(unsigned int infected, unsigned int uninfected);
    void addInfectionRate(double rate);
    double calculateRate();
};

}


#endif