#ifndef TRANSMODEL_PARAMUPDATER_H
#define TRANSMODEL_PARAMUPDATER_H

#include "repast_hpc/Schedule.h"

#include "Parameters.h"


namespace TransModel {

class Model;

void schedule_parameter_interventions(repast::ScheduleRunner& runner);

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


} // namespace TransModel

#endif // TRANSMODEL_PARAMUPDATER_H
