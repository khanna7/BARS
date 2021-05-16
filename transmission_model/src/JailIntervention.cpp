#include "JailIntervention.h"

#include "boost/tokenizer.hpp"
#include "boost/algorithm/string.hpp"

#include "Parameters.h"
#include "Model.h"

namespace TransModel {

using funcP = repast::Schedule::FunctorPtr;

void parse_intervention_string(const std::string& param, double* val, double* at) {
    *at = -1;
    if (Parameters::instance()->contains(param)) {
        std::string p_value = Parameters::instance()->getStringParameter(param);
        boost::char_separator<char> sep("|");
        boost::tokenizer<boost::char_separator<char>> ctok(p_value, sep);
        int idx = 0;
        for (auto item : ctok) {
            boost::trim(item);
            if (idx == 0) {
                *at = std::stod(item);
                ++idx;
            } else if (idx == 1) {
                *val = std::stod(item);
                idx++;
            }
        }
        if (idx != 2) {
            throw std::invalid_argument("Bad parameter value for " + param + ". Expected \"at|value\".");
        }
    }
}

void parse_intervention_string(const std::string& param, bool* val, double* at) {
    *at = -1;
    if (Parameters::instance()->contains(param)) {
        std::string p_value = Parameters::instance()->getStringParameter(param);
        boost::char_separator<char> sep("|");
        boost::tokenizer<boost::char_separator<char>> ctok(p_value, sep);
        int idx = 0;
        for (auto item : ctok) {
            boost::trim(item);
            if (idx == 0) {
                *at = std::stod(item);
                ++idx;
            } else if (idx == 1) {
                if (item == "TRUE" || item == "true" || item == "T") {
                    *val = true;
                } else {
                    *val = false;
                }
                idx++;
            }
        }
        if (idx != 2) {
            throw std::invalid_argument("Bad parameter value for " + param + ". Expected \"at|value\".");
        }
    }
}

JailInfRateUpdater::JailInfRateUpdater(double inf_rate, Jail* jail) : inf_rate_{inf_rate}, jail_{jail} {}
void JailInfRateUpdater::operator()() {
   jail_->updateInfRateMult(inf_rate_);
}

void schedule_jail_interventions(repast::ScheduleRunner& runner, Model* model, Jail* jail) {
    double val, at;
    parse_intervention_string(JAIL_INFECTION_RATE_MULTIPLIER_UPDATE, &val, &at);
    if (at != -1) {
        std::cout << "Scheduling jail infection rate multipler update to " << val << " at " << at << std::endl;
        runner.scheduleEvent(at - 0.00001, funcP(new JailInfRateUpdater(val, jail)));
    }

    parse_intervention_string(INCARCERATION_PROB_UPDATE, &val, &at);
    if (at != -1) {
        std::cout << "Scheduling incarceration probability update to " << val << " at " << at << std::endl;
        runner.scheduleEvent(at - 0.00001, funcP(new ParamUpdater<double>(val, INCARCERATION_PROB)));
    }

    parse_intervention_string(INCARCERATION_WITH_CJI_PROB_UPDATE, &val, &at);
    if (at != -1) {
        std::cout << "Scheduling incarceration cji probability update to " << val << " at " << at << std::endl;
        runner.scheduleEvent(at - 0.00001, funcP(new ParamUpdater<double>(val, INCARCERATION_WITH_CJI_PROB)));
    }

    parse_intervention_string(JAIL_SERVING_TIME_MEAN_UPDATE, &val, &at);
    if (at != -1) {
        std::cout << "Scheduling jail serving time mean update to " << val << " at " << at << std::endl;
        runner.scheduleEvent(at - 0.00001, funcP(new ParamUpdater<double>(val, JAIL_SERVING_TIME_MEAN)));
    }
    
    parse_intervention_string(NETWORK_RETENTION_MULTIPLIER_UPDATE, &val, &at);
    if (at != -1) {
        std::cout << "Scheduling network retention multipler update to " << val << " at " << at << std::endl;
        runner.scheduleEvent(at - 0.00001, funcP(new ParamUpdater<double>(val, NETWORK_RETENTION_MULTIPLIER)));
    }

    parse_intervention_string(POST_RELEASE_INTERFERENCE_PERIOD_MEAN_UPDATE, &val, &at);
    if (at != -1) {
        std::cout << "Scheduling post release interference period mean update to " << val << " at " << at << std::endl;
        runner.scheduleEvent(at - 0.00001, funcP(new ParamUpdater<double>(val, POST_RELEASE_INTERFERENCE_PERIOD_MEAN)));
    }

    bool b = false;
    parse_intervention_string(IS_NETWORK_DISRUPTION_ON_UPDATE, &b, &at);
    if (at != -1) {
        std::cout << "Scheduling network disruption on update to " << (b ? "true" : "false") << " at " << at << std::endl;
        runner.scheduleEvent(at - 0.00001, funcP(new ParamUpdater<bool>(b, IS_NETWORK_DISRUPTION_ON)));
    }

    parse_intervention_string(IS_CARE_DISRUPTION_ON_UPDATE, &b, &at);
    if (at != -1) {
        std::cout << "Scheduling care disruption on update update to " << (b ? "true" : "false") << " at " << at << std::endl;
        runner.scheduleEvent(at - 0.00001, funcP(new ParamUpdater<bool>(b, IS_CARE_DISRUPTION_ON)));
    }

    parse_intervention_string(IS_POST_RELEASE_ART_ON_UPDATE, &b, &at);
    if (at != -1) {
        std::cout << "Scheduling post release ART on update update to " << (b ? "true" : "false") << " at " << at << std::endl;
        runner.scheduleEvent(at - 0.00001, funcP(new ParamUpdater<bool>(b, IS_POST_RELEASE_ART_ON)));
    }
}

}
