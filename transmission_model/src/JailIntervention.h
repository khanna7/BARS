#ifndef SRC_JAILINTERVENTION_H
#define SRC_JAILINTERVENTION_H

#include "repast_hpc/Schedule.h"

#include "Parameters.h"


namespace TransModel {

class Model;
class Jail;

void schedule_jail_interventions(repast::ScheduleRunner& runner, Model* model, Jail* jail);

class JailInfRateUpdater : public repast::Functor {

private:
    double inf_rate_;
    Jail* jail_;

public:
    JailInfRateUpdater(double inf_rate, Jail* jail);
    virtual ~JailInfRateUpdater() {}
    void operator()();
};

template <typename param_type>
class ParamUpdater : public repast::Functor {

private:
    param_type val_;
    std::string param_;

public:
    ParamUpdater(param_type val, const std::string& param);
    virtual ~ParamUpdater() {}
    void operator()();
};

template<typename param_type>
ParamUpdater<param_type>::ParamUpdater(param_type val, const std::string& param) : val_{val}, param_{param} {}

template<typename param_type>
void ParamUpdater<param_type>::operator()() {
    Parameters::instance()->putParameter(param_, val_);
}



}

#endif