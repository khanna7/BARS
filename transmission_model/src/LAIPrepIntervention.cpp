#include "repast_hpc/Random.h"

#include "LAIPrepIntervention.h"
#include "Parameters.h"

namespace TransModel {

LAIPrepIntervention::LAIPrepIntervention(PrepUptakeData& prep_data) : PrepIntervention(prep_data),
    candidates() {
    onYearEnded();
}

LAIPrepIntervention::~LAIPrepIntervention() {
    
}

void LAIPrepIntervention::reset() {
    candidates.clear();
}

void LAIPrepIntervention::processPerson(std::shared_ptr<Person> &person, Network<Person> &network) {
    if (!person->isOnPrep()) {
        candidates.push_back(person);
    }
}

void LAIPrepIntervention::run(double tick, std::vector<PersonPtr> &put_on_prep, Network<Person> &network) {
    if (candidates.size() > 0) {
        double prep_p = Parameters::instance()->getDoubleParameter(LAI_PREP_USE_PROB);
        double prop_lai = Parameters::instance()->getDoubleParameter(LAI_PREP_LAI_PROP );
        for (auto& person : candidates) {
            if (repast::Random::instance()->nextDouble() <= prep_p) {
                PrepStatus status = PrepStatus::ON;
                if (repast::Random::instance()->nextDouble() <= prop_lai) {
                    status = PrepStatus::ON_LAI;
                }
                putOnPrep(tick, person, status);
                put_on_prep.push_back(person);
            }
        }
    }
}

}
