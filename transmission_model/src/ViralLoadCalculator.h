/*
 * ViralLoadCalculator.h
 *
 *  Created on: Dec 3, 2015
 *      Author: nick
 */

#ifndef SRC_VIRALLOADCALCULATOR_H_
#define SRC_VIRALLOADCALCULATOR_H_

#include "DiseaseParameters.h"

namespace TransModel {

class ViralLoadCalculator {

private:
    SharedViralLoadParameters vl_params;
    float calculateViralLoadNoART(const InfectionParameters& infection_params);
    float calculateViralLoadART(const InfectionParameters& infection_params);

public:
    ViralLoadCalculator(SharedViralLoadParameters& params);
    virtual ~ViralLoadCalculator();

    float calculateViralLoad(const InfectionParameters& infection_params);

    const SharedViralLoadParameters& sharedViralLoadParameters() const {
        return vl_params;
    }

};

} /* namespace TransModel */

#endif /* SRC_VIRALLOADCALCULATOR_H_ */
