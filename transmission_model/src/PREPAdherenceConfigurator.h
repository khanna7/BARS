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
    ProbDist<AdherenceData> lt_dist_, gte_dist_, meth_dist_, crack_dist_, ecstasy_dist_,
    meth_crack_dist_, crack_ecstasy_dist_, meth_crack_ecstasy_dist_;
    float age_threshold_;
    std::map<AdherenceCategory, double> cat_map_;
    
    static PREPAdherenceConfigurator* instance_;

    PREPAdherenceConfigurator();

public:
    virtual ~PREPAdherenceConfigurator();

    static PREPAdherenceConfigurator* instance();

    void configurePerson(std::shared_ptr<Person> person);
    void configurePerson(std::shared_ptr<Person> person, double draw);
    void configurePerson(std::shared_ptr<Person> person, AdherenceCategory category);
};

} /* namespace TransModel */

#endif /* SRC_PREPADHERENCECONFIGURATOR_H_ */
