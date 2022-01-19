#include "DrugUse.h"
#include "DrugUseCessationEvent.h"
#include "adherence_functions.h"
#include "Parameters.h"

using namespace repast;

namespace TransModel {

void DrugUse::goOnOffDrug(PersonPtr& person) {
    initialize_art_adherence(person, -1);
    PREPAdherenceConfigurator::instance()->configurePerson(person);
}

DrugUse::DrugUse(SubstanceUseType drug, double proportion_on, int length) :
    cessation_generator( 1. / length, 1.1), proportion_on_(proportion_on),
    length_(length), drug_(drug), candidates(), total_pop(0) {
}

DrugUse::
~DrugUse() {}

void DrugUse::reset() {
    candidates.clear();
    total_pop = 0;
}

void DrugUse::processPerson(std::shared_ptr<Person> &person) {
    ++total_pop;
    if (!person->isSubstanceUser(drug_)) {
        candidates.push_back(person);
    }
}

void DrugUse::run(double tick) {
    double use_prob = 0;
    if (candidates.size() > 0) {
        use_prob = (double)total_pop * proportion_on_ / ((double)length_ * candidates.size());
        cout << "proportion target: " << proportion_on_ << "; actual: "  << 1. - candidates.size()/(double)total_pop << endl;
        for (auto& person: candidates) {
            if (Random::instance()->nextDouble() <= use_prob) {
                putOnDrug(tick, person);
            }
        }
    }
}

void DrugUse::putOnDrug(double tick, std::shared_ptr<Person> &person) {
    person->goOnDrug(drug_);
    goOnOffDrug(person);
    ScheduleRunner& runner = RepastProcess::instance()->getScheduleRunner();
    double delay = cessation_generator.next();
    double stop_time = tick + delay;
    runner.scheduleEvent(stop_time, Schedule::FunctorPtr(new DrugUseCessationEvent(person, drug_, stop_time)));
}

}
