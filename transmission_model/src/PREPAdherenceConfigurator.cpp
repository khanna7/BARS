/*
 * PREPAdherenceConfigurator.cpp
 *
 *  Created on: Oct 12, 2017
 *      Author: nick
 */

#include "Parameters.h"

#include "PREPAdherenceConfigurator.h"

namespace TransModel {

void init_category_map(std::map<AdherenceCategory, double>& cat_map) {
    cat_map.emplace(AdherenceCategory::ALWAYS, Parameters::instance()->getDoubleParameter(PREP_ALWAYS_ADHERENT_TR));
    cat_map.emplace(AdherenceCategory::NEVER, Parameters::instance()->getDoubleParameter(PREP_NEVER_ADHERENT_TR));
    cat_map.emplace(AdherenceCategory::PARTIAL_PLUS, Parameters::instance()->getDoubleParameter(PREP_PARTIAL_POS_ADHERENT_TR));
    cat_map.emplace(AdherenceCategory::PARTIAL_MINUS, Parameters::instance()->getDoubleParameter(PREP_PARTIAL_NEG_ADHERENT_TR));
}

ProbDist<AdherenceData> create_prep_adherence_dist(const std::string& threshold_suffix) {
    double always = Parameters::instance()->getDoubleParameter(PREP_PROP_ALWAYS_ADHERENT + threshold_suffix);
    double never = Parameters::instance()->getDoubleParameter(PREP_PROP_NEVER_ADHERENT + threshold_suffix);
    double partial_plus = Parameters::instance()->getDoubleParameter(PREP_PROP_PARTIAL_POS_ADHERENT + threshold_suffix);
    double partial_minus = Parameters::instance()->getDoubleParameter(PREP_PROP_PARTIAL_NEG_ADHERENT + threshold_suffix);

    double always_prob = Parameters::instance()->getDoubleParameter(PREP_ALWAYS_ADHERENT_TR);
    double never_prob = Parameters::instance()->getDoubleParameter(PREP_NEVER_ADHERENT_TR);
    double partial_plus_prob = Parameters::instance()->getDoubleParameter(PREP_PARTIAL_POS_ADHERENT_TR);
    double partial_minus_prob = Parameters::instance()->getDoubleParameter(PREP_PARTIAL_NEG_ADHERENT_TR);

    //std::cout << "prep: " << always << "," << never << "," << partial_plus << ", " << partial_minus << std::endl;

    ProbDistCreator<AdherenceData> creator;
    creator.addItem(always, AdherenceData(always_prob, AdherenceCategory::ALWAYS));
    creator.addItem(never, AdherenceData(never_prob, AdherenceCategory::NEVER));
    creator.addItem(partial_plus, AdherenceData(partial_plus_prob, AdherenceCategory::PARTIAL_PLUS));
    creator.addItem(partial_minus, AdherenceData(partial_minus_prob, AdherenceCategory::PARTIAL_MINUS));

    return creator.createProbDist();
}

PREPAdherenceConfigurator *PREPAdherenceConfigurator::instance_ = nullptr;

PREPAdherenceConfigurator* PREPAdherenceConfigurator::instance() {
    if (instance_ == nullptr) {
        instance_ = new PREPAdherenceConfigurator();
    }
    return instance_;
}

PREPAdherenceConfigurator::PREPAdherenceConfigurator() :
    lt_dist_{create_prep_adherence_dist(LT_SUFFIX)},
    gte_dist_{create_prep_adherence_dist(GTE_SUFFIX)},
    meth_dist_{ create_prep_adherence_dist(METH_SUFFIX)},
    crack_dist_{create_prep_adherence_dist(CRACK_SUFFIX)},
    ecstasy_dist_{create_prep_adherence_dist(ECSTASY_SUFFIX)},
    meth_crack_dist_{create_prep_adherence_dist(METH_SUFFIX + CRACK_SUFFIX)},
    crack_ecstasy_dist_{create_prep_adherence_dist(CRACK_SUFFIX + ECSTASY_SUFFIX)},
    meth_crack_ecstasy_dist_{
        create_prep_adherence_dist(METH_SUFFIX + CRACK_SUFFIX + ECSTASY_SUFFIX)} {
    
    //float age_threshold = Parameters::instance()->getFloatParameter(INPUT_AGE_THRESHOLD);
    init_category_map(cat_map_);
}

PREPAdherenceConfigurator::~PREPAdherenceConfigurator() {
}

void PREPAdherenceConfigurator::configurePerson(std::shared_ptr<Person> person, double draw) {
    AdherenceData data;
    if (person->isSubstanceUser(SubstanceUseType::METH) &&
            person->isSubstanceUser(SubstanceUseType::CRACK) &&
            person->isSubstanceUser(SubstanceUseType::ECSTASY)) {
        data = meth_crack_ecstasy_dist_.draw(draw);
    } else if (person->isSubstanceUser(SubstanceUseType::METH) &&
               person->isSubstanceUser(SubstanceUseType::CRACK)) {
        data = meth_crack_dist_.draw(draw);
    } else if (person->isSubstanceUser(SubstanceUseType::CRACK) &&
               person->isSubstanceUser(SubstanceUseType::ECSTASY)) {
        data = crack_ecstasy_dist_.draw(draw);
    } else if (person->isSubstanceUser(SubstanceUseType::METH)) {
        data = meth_dist_.draw(draw);
    } else if (person->isSubstanceUser(SubstanceUseType::CRACK)) {
        data = crack_dist_.draw(draw);
    } else if (person->isSubstanceUser(SubstanceUseType::ECSTASY)) {
        data = ecstasy_dist_.draw(draw);
    } else {
        data = person->age() < age_threshold_ ? lt_dist_.draw(draw) : gte_dist_.draw(draw);
    }
    person->updatePrepAdherence(data);
}

void PREPAdherenceConfigurator::configurePerson(std::shared_ptr<Person> person) {
    double draw = repast::Random::instance()->nextDouble();
    configurePerson(person, draw);
}

void PREPAdherenceConfigurator::configurePerson(std::shared_ptr<Person> person, AdherenceCategory category) {
    double prob = cat_map_.at(category);
    AdherenceData data(prob, category);
    person->updatePrepAdherence(data);
}

} /* namespace TransModel */
