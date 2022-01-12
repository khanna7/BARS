#include "MethUse.h"
#include "MethUseCessationEvent.h"
#include "adherence_functions.h"
#include "Parameters.h"

using namespace repast;

namespace TransModel {

void goOnOffSubstance(PersonPtr person) {
    initialize_art_adherence(person, -1);
    PREPAdherenceConfigurator::instance()->configurePerson(person);
}

void goOnMeth(PersonPtr person) {
    cout << person->id() << " Going on METH" << endl;
    person->goOnMeth();
    goOnOffSubstance(person);


}

MethUse::MethUse(double proportion_on, int length) :
    cessation_generator( 1. / length, 1.1), proportion_on_(proportion_on),
    length_(length), candidates(), total_pop(0) {
}

MethUse::
~MethUse() {}

void MethUse::reset() {
    candidates.clear();
    total_pop = 0;
}

void MethUse::processPerson(std::shared_ptr<Person> &person) {
    ++total_pop;
    if (!person->isSubstanceUser(SubstanceUseType::METH)) {
        candidates.push_back(person);
    }
}

void MethUse::run(double tick) {
    double use_prob = 0;
    if (candidates.size() > 0) {
        use_prob = (double)total_pop * proportion_on_ / ((double)length_ * candidates.size());
        cout << "length " << length_ << " meth prob: " << use_prob << " proportion on " << proportion_on_ << " (num cand / total) " << 1. - candidates.size()/(double)total_pop << endl;
        for (auto& person: candidates) {
            if (Random::instance()->nextDouble() <= use_prob) {
                putOnMeth(tick, person);
            }
        }
    }
}

void MethUse::putOnMeth(double tick, std::shared_ptr<Person> &person) {
    ScheduleRunner& runner = RepastProcess::instance()->getScheduleRunner();
    double delay = cessation_generator.next();
    double stop_time = tick + delay;
    goOnMeth(person);
    std::cout << "Use length " << delay << std::endl;
    runner.scheduleEvent(stop_time, Schedule::FunctorPtr(new MethUseCessationEvent(person, stop_time)));
}

}
