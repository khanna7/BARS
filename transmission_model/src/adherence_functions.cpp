/*
 * art_functions.cpp
 *
 *  Created on: Sep 26, 2016
 *      Author: nick
 */


#include "repast_hpc/RepastProcess.h"

#include "adherence_functions.h"
#include "AdherenceCategory.h"
#include "AdherenceCheckScheduler.h"
#include "Parameters.h"
#include "ProbDist.h"

namespace TransModel {

void initialize_art_adherence(std::shared_ptr<Person> person, double tick, AdherenceCategory category) {
    double prob = 0;
    if (category == AdherenceCategory::ALWAYS) {
        prob = Parameters::instance()->getDoubleParameter(ART_ALWAYS_ADHERENT_PROB);
    } else if (category == AdherenceCategory::NEVER) {
        prob = Parameters::instance()->getDoubleParameter(ART_NEVER_ADHERENT_PROB);
    } else if (category == AdherenceCategory::PARTIAL_PLUS) {
        prob = Parameters::instance()->getDoubleParameter(ART_PARTIAL_POS_ADHERENT_PROB);
    }  else if (category == AdherenceCategory::PARTIAL_MINUS) {
        prob = Parameters::instance()->getDoubleParameter(ART_PARTIAL_NEG_ADHERENT_PROB);
    }

    person->setArtAdherence({prob, category});
    schedule_art_adherence(person, tick);
}

void initialize_art_adherence(std::shared_ptr<Person> person, double first_art_at_tick) {
    std::string suffix = "";
    if (person->isSubstanceUser(SubstanceUseType::METH)) {
        suffix = METH_SUFFIX;
    } else if (person->isSubstanceUser(SubstanceUseType::CRACK)) {
        suffix = CRACK_SUFFIX;
    } else if (person->isSubstanceUser(SubstanceUseType::ECSTASY)) {
        suffix = ECSTASY_SUFFIX;
    }

    double always = Parameters::instance()->getDoubleParameter(ART_PROP_ALWAYS_ADHERENT + suffix);
    double never = Parameters::instance()->getDoubleParameter(ART_PROP_NEVER_ADHERENT + suffix);
    double partial_plus = Parameters::instance()->getDoubleParameter(ART_PROP_PARTIAL_POS_ADHERENT + suffix);
    double partial_minus = Parameters::instance()->getDoubleParameter(ART_PROP_PARTIAL_NEG_ADHERENT + suffix);

    //std::cout << "art: " << always << "," << never << "," << partial_plus << ", " << partial_minus << std::endl;

    double always_prob = Parameters::instance()->getDoubleParameter(ART_ALWAYS_ADHERENT_PROB);
    double never_prob = Parameters::instance()->getDoubleParameter(ART_NEVER_ADHERENT_PROB);
    double partial_plus_prob = Parameters::instance()->getDoubleParameter(ART_PARTIAL_POS_ADHERENT_PROB);
    double partial_minus_prob = Parameters::instance()->getDoubleParameter(ART_PARTIAL_NEG_ADHERENT_PROB);

    ProbDistCreator<AdherenceData> creator;
    creator.addItem(always, AdherenceData(always_prob, AdherenceCategory::ALWAYS));
    creator.addItem(never, AdherenceData(never_prob, AdherenceCategory::NEVER));
    creator.addItem(partial_plus, AdherenceData(partial_plus_prob, AdherenceCategory::PARTIAL_PLUS));
    creator.addItem(partial_minus, AdherenceData(partial_minus_prob, AdherenceCategory::PARTIAL_MINUS));

    AdherenceData data = creator.createProbDist().draw(repast::Random::instance()->nextDouble());
    person->setArtAdherence(data);
    schedule_art_adherence(person, first_art_at_tick);
}

void schedule_art_adherence(std::shared_ptr<Person> person, double first_art_at_tick) {
    double window_length = Parameters::instance()->getDoubleParameter(PARTIAL_ART_ADHER_WINDOW_LENGTH);
    double remainder = window_length;
    if (!isnan(person->infectionParameters().time_since_art_init)) {
        // can be on ART from burnin in which case need to schedule 
        // at the update at the appropriate time.
        remainder = window_length - (((int)person->infectionParameters().time_since_art_init) % ((int)window_length));
        if (remainder < 0) {
            remainder = 1;
        } else if (remainder > window_length) {
            remainder = window_length;
        }
    }
    double adherence_check_at = first_art_at_tick + remainder;
    ARTAdherenceCheckScheduler* ac_scheduler = new ARTAdherenceCheckScheduler(person, adherence_check_at);
    repast::RepastProcess::instance()->getScheduleRunner().scheduleEvent(adherence_check_at - 0.1,
            repast::Schedule::FunctorPtr(ac_scheduler));
}

}

