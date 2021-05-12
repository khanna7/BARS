/*
 * ARTScheduler.cpp
 *
 *  Created on: Feb 17, 2016
 *      Author: nick
 */

#include "Parameters.h"

#include "ARTScheduler.h"

#include "adherence_functions.h"
#include "Stats.h"

//#include "EventWriter.h"
//#include "Events.h"

namespace TransModel {

ARTScheduler::ARTScheduler(float time_stamp) :
        time_stamp_(time_stamp), persons { } {
}

ARTScheduler::~ARTScheduler() {
}

void ARTScheduler::operator()() {
    for (auto& p : persons) {
        // person might be die in between ART is scheduled
        // and actually going on ART.
        if (!p->isDead()) {
            if (p->artAdherence().category == AdherenceCategory::NA) {
                initialize_art_adherence(p, time_stamp_);
                p->goOnART(time_stamp_);
                Stats::instance()->personDataRecorder()->recordARTStart(p, time_stamp_);
                Stats::instance()->recordARTEvent(time_stamp_, p->id(), true);
                if (p->adheringToCBTreatment() || p->adheringToMirtazapineTreatment()) {
                    p->setARTAdherenceBeforeTreatment(p->artAdherence());
                    double prob = Parameters::instance()->getDoubleParameter(
                                ART_ALWAYS_ADHERENT_PROB);
                    p->setArtAdherence({prob, AdherenceCategory::ALWAYS});
                } else if (p->onCounselingAndBehavioralTreatment()) {
                    p->setARTAdherenceBeforeTreatment(p->artAdherence());
                    double prob = Parameters::instance()->getDoubleParameter(
                                ART_PARTIAL_POS_ADHERENT_PROB);
                    p->setArtAdherence({prob, AdherenceCategory::PARTIAL_PLUS});
                }
            }
        }
    }
}

void ARTScheduler::addPerson(PersonPtr person) {
    persons.push_back(person);
}


ARTPostBurninScheduler::ARTPostBurninScheduler(float time_stamp, PersonPtr p) : time_stamp_(time_stamp), person(p) {}

void ARTPostBurninScheduler::operator()() {
    // person might be die in between ART is scheduled
    // and actually going on ART.
    if (!person->isDead()) {
        initialize_art_adherence(person, time_stamp_);
        if (person->onCounselingAndBehavioralTreatment()) {
            person->setARTAdherenceBeforeTreatment(person->artAdherence());
            double prob = Parameters::instance()->getDoubleParameter(
                        ART_ALWAYS_ADHERENT_PROB);
            person->setArtAdherence({prob, AdherenceCategory::ALWAYS});
        }
        person->goOnART(time_stamp_);
        Stats::instance()->personDataRecorder()->recordARTStart(person, time_stamp_);
        Stats::instance()->recordARTEvent(time_stamp_, person->id(), true);
    }
}

} /* namespace TransModel */
