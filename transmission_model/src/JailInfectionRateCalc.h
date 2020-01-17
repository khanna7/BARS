#ifndef SRC_JAILINFECTIONRATECALC_H_
#define SRC_JAILINFECTIONRATECALC_H_

#include <list>

class JailInfRateCalculator  {

private:
    std::list<double> rates;
    unsigned int window_size_;
    double multiplier_;
    double default_rate_;

public:
    JailInfRateCalculator(unsigned int window_size, double multiplier, double default_rate);
    ~JailInfRateCalculator();
    void addInfectionRate(unsigned int infected, unsigned int uninfected);
    double calculateRate();
};


#endif