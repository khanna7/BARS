#include "CounselingAndBehavioralTreatmentIntervention.h"

#include "CounselingAndBehavioralTreatmentCessationEvent.h"

using namespace repast;

namespace TransModel {

CounselingAndBehavioralTreatmentIntervention::
CounselingAndBehavioralTreatmentIntervention(double proportion_on, int length) :
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
    person->goOnCounselingAndBehavioralTreatment(tick, stop_time);
    runner.scheduleEvent(stop_time, Schedule::FunctorPtr(new CounselingAndBehavioralTreatmentCessationEvent(person, stop_time)));
}

}
