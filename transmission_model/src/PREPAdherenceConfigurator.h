/*
 * PREPAdherenceConfigurator.h
 *
 *  Created on: Oct 12, 2017
 *      Author: nick
 */

#ifndef SRC_PREPADHERENCECONFIGURATOR_H_
#define SRC_PREPADHERENCECONFIGURATOR_H_

#include <memory>
#include <map>

#include "ProbDist.h"
#include "AdherenceCategory.h"
#include "Person.h"


namespace TransModel {

class PREPAdherenceConfigurator {
private:
    ProbDist<AdherenceData> lt_dist_, gte_dist_;
    float age_threshold_;
    std::map<AdherenceCategory, double> cat_map_;
public:
    PREPAdherenceConfigurator(ProbDist<AdherenceData> lt_dist, ProbDist<AdherenceData> gte_dist, float age_threshold,
            std::map<AdherenceCategory, double> cat_map);
    virtual ~PREPAdherenceConfigurator();

    void configurePerson(std::shared_ptr<Person> person);
    void configurePerson(std::shared_ptr<Person> person, double draw);
    void configurePerson(std::shared_ptr<Person> person, AdherenceCategory category);
};

PREPAdherenceConfigurator create_prep_adherence_configurator();


} /* namespace TransModel */

#endif /* SRC_PREPADHERENCECONFIGURATOR_H_ */
