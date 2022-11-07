#include "ParamUpdater.h"

#include "boost/tokenizer.hpp"
#include "boost/algorithm/string.hpp"

#include "Parameters.h"

namespace TransModel {

using funcP = repast::Schedule::FunctorPtr;

void parse_intervention_string(const std::string& param, double* val, double* at) {
    *at = -1;
    std::cout << "Param string: " << param << std::endl;
    if (Parameters::instance()->contains(param)) {
        std::cout << "Found parameter" << std::endl;
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

void schedule_parameter_interventions(repast::ScheduleRunner& runner) {
    double val, at;
    parse_intervention_string(LAI_PREP_LAI_PROP_UPDATE, &val, &at);
    if (at != -1) {
        std::cout << "Scheduling lai prep lai proportion update to " << val << " at " << at << std::endl;
        runner.scheduleEvent(at - 0.00001, funcP(new ParamUpdater<double>(val, LAI_PREP_LAI_PROP)));
    }
}

} // namespace TransModel
