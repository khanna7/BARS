#include "CounselingAndBehavioralTreatmentIntervention.h"
#include "CounselingAndBehavioralTreatmentCessationEvent.h"
#include "adherence_functions.h"
#include "Parameters.h"

using namespace repast;

namespace TransModel {

void goOnCounselingAndBehavioralTreatment(PersonPtr person, double tick, double stop_time) {
    //cout << person->id() << " Going on BC" << endl;
    person->setOnCounselingAndBehavioralTreatment(true);
    bool adhering = false;
    if (Random::instance()->nextDouble() <=
            Parameters::instance()->getDoubleParameter(
                COUNSELING_AND_BEHAVIORAL_TREATMENT_ADHERENCE_PROP)) {
        adhering = true;
        person->setAdheringToCBTreatment(true);
    }
    if (person->isInfected()) {
        if (!person->isDiagnosed()) {
            person->setDiagnosed(tick);
        }
        if (person->artAdherence().category == AdherenceCategory::NA) {
            initialize_art_adherence(person, tick);
        }
        if (!person->isOnART(false)) {
            person->goOnART(tick);
            Stats::instance()->personDataRecorder()->recordARTStart(person, tick);
            Stats::instance()->recordARTEvent(tick, person->id(), true);
        }
        person->setARTAdherenceBeforeTreatment(person->artAdherence());
        if (adhering) {
            double prob = Parameters::instance()->getDoubleParameter(
                        ART_ALWAYS_ADHERENT_PROB);
            person->setArtAdherence({prob, AdherenceCategory::ALWAYS});
        } else {
            double prob = Parameters::instance()->getDoubleParameter(
                        ART_PARTIAL_POS_ADHERENT_PROB);
            person->setArtAdherence({prob, AdherenceCategory::PARTIAL_PLUS});
        }
    } else {
        if (!person->isOnPrep(false)) {
            person->goOnPrep(tick, stop_time);
            Stats* stats = Stats::instance();
            stats->recordPREPEvent(tick, person->id(),
                                   static_cast<int>(PrepStatus::ON_TREATMENT),
                                   person->isSubstanceUser(SubstanceUseType::METH),
                                   person->isSubstanceUser(SubstanceUseType::CRACK),
                                   person->isSubstanceUser(SubstanceUseType::ECSTASY));
            stats->personDataRecorder()->recordPREPStart(person, tick);
        }
        person->setPrepBeforeTreatment(person->prepParameters());
        if (adhering) {
            person->setPrepParametersAdherenceData({Parameters::instance()->getDoubleParameter(
                                                    PREP_ALWAYS_ADHERENT_TR),
                                                    AdherenceCategory::ALWAYS});
        } else {
            person->setPrepParametersAdherenceData({Parameters::instance()->getDoubleParameter(
                                                     PREP_PARTIAL_POS_ADHERENT_TR),
                                                     AdherenceCategory::PARTIAL_PLUS});
        }
    }
}

CounselingAndBehavioralTreatmentIntervention::CounselingAndBehavioralTreatmentIntervention(double proportion_on, int length) :
    cessation_generator( 1. / length, 1.1), proportion_on_(proportion_on),
    length_(length), candidates(), total_substance_users(0) {
}

CounselingAndBehavioralTreatmentIntervention::
~CounselingAndBehavioralTreatmentIntervention() {}

void CounselingAndBehavioralTreatmentIntervention::reset() {
    candidates.clear();
    total_substance_users = 0;
}

void CounselingAndBehavioralTreatmentIntervention::processPerson(std::shared_ptr<Person> &person) {
    if (person->isSubstanceUser()) {
        ++total_substance_users;
        if (!person->onCounselingAndBehavioralTreatment()) {
            candidates.push_back(person);
        }
    }
}

void CounselingAndBehavioralTreatmentIntervention::run(double tick) {
    double treatment_prob = 0;
    if (candidates.size() > 0) {
        treatment_prob = (double)total_substance_users * proportion_on_ / ((double)length_ * candidates.size());
        cout << "treatment prob: " << treatment_prob << " proportion on " << proportion_on_ << " (num cand / total) " << candidates.size()/(double)total_substance_users << endl;
        for (auto& person: candidates) {
            if (Random::instance()->nextDouble() <= treatment_prob) {
                putOnTreatment(tick, person);
            }
        }
    }
}

void CounselingAndBehavioralTreatmentIntervention::putOnTreatment(double tick, std::shared_ptr<Person> &person) {
    ScheduleRunner& runner = RepastProcess::instance()->getScheduleRunner();
    double delay = cessation_generator.next();
    double stop_time = tick + delay;
    goOnCounselingAndBehavioralTreatment(person, tick, stop_time);
    runner.scheduleEvent(stop_time, Schedule::FunctorPtr(new CounselingAndBehavioralTreatmentCessationEvent(person, stop_time)));
}

}
