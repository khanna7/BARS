#include "MirtazapineTreatmentIntervention.h"
#include "MirtazapineTreatmentCessationEvent.h"

using namespace repast;

namespace TransModel {

MirtazapineIntervention::MirtazapineIntervention(double proportion_on,
                                                 int length) :
    cessation_generator( 1. / length, 1.1), proportion_on_(proportion_on),
    length_(length), candidates(), total_meth_users(0) {
}

MirtazapineIntervention::~MirtazapineIntervention() {}

void MirtazapineIntervention::reset() {
    candidates.clear();
    total_meth_users = 0;
}

void MirtazapineIntervention::processPerson(std::shared_ptr<Person> &person) {
    if (person->isSubstanceUser(SubstanceUseType::METH)) {
        ++total_meth_users;
        if (!person->onMirtazapineTreatment()) {
            candidates.push_back(person);
        }
    }
}

void MirtazapineIntervention::run(double tick) {
    double treatment_prob = 0;
    if (candidates.size() > 0) {
        treatment_prob = (double)total_meth_users * proportion_on_ / ((double)length_ * candidates.size());
        //cout << "treatment prob: " << treatment_prob << " target proportion on " << proportion_on_ << " current_proportion_on " << (total_meth_users -  candidates.size())/(double)total_meth_users << endl;
        for (auto& person: candidates) {
            if (Random::instance()->nextDouble() <= treatment_prob) {
                putOnTreatment(tick, person);
            }
        }
    }
}

void MirtazapineIntervention::putOnTreatment(double tick, std::shared_ptr<Person> &person) {
    ScheduleRunner& runner = RepastProcess::instance()->getScheduleRunner();
    double delay = cessation_generator.next();
    double stop_time = tick + delay;
    person->goOnMirtazapineTreatment(tick, stop_time);
    runner.scheduleEvent(stop_time, Schedule::FunctorPtr(new MirtazapineTreatmentCessationEvent(person, stop_time)));
}

}
