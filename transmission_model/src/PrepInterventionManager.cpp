/**
 * PrepInterventionManager.cpp
 */

#include "PrepInterventionManager.h"

#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Schedule.h"

#include "Stats.h"
#include "PrepCessationEvent.h"

using namespace repast;

namespace TransModel {

std::ostream& operator <<(std::ostream& stream, const PrepUptakeData& prep_data) {
    stream << "PrepUptakeData [\nUse (k): " << prep_data.use << "\nStop (p): " << prep_data.stop << "\nCessation Stop: " << 
    prep_data.cessation_stop << "\nIncrement: " << prep_data.increment << "\nYears to Increment: " << 
    prep_data.years_to_increment << "\n]";
    return stream;
}


bool gte(PersonPtr person, double threshold) {
    return person->age() >= threshold;
}

bool lt(PersonPtr person, double threshold) {
    return person->age() < threshold;
}

PrepIntervention::PrepIntervention(PrepUptakeData& data, std::vector<std::shared_ptr<PrepFilter>> filters) : IPrepIntervention(), cessation_generator(data.cessation_stop, 1.1), year(1)
{
    for (auto filter : filters) {
        addFilter(filter);
    }
}

PrepIntervention::~PrepIntervention() {}

void PrepIntervention::addFilter(std::shared_ptr<PrepFilter> filter) {
    std::shared_ptr<PrepFilter> f(filter);
    filters_.push_back(f);
}

bool PrepIntervention::runFilters(std::shared_ptr<Person> person) {
    bool passed = true;
    for (auto filter : filters_) {
        if (!filter->apply(person)) {
            passed = false;
            break;
        }
    }
    return passed;
}

double PrepIntervention::calcPrepStopAdjustment()
{
    double adjustment = 0.0;
    for (auto filter : filters_) {
        adjustment += filter->calcPrepStopAdjustment();
    }
    return adjustment;
}

void PrepIntervention::putOnPrep(double tick, std::shared_ptr<Person>& person, PrepStatus cause) {
    ScheduleRunner& runner = RepastProcess::instance()->getScheduleRunner();
    double delay = cessation_generator.next();
    double stop_time = tick + delay;
    person->goOnPrep(tick, stop_time);
    Stats* stats = Stats::instance();
    stats->recordPREPEvent(tick, person->id(), static_cast<int>(cause), person->isSubstanceUser(SubstanceUseType::METH),
                           person->isSubstanceUser(SubstanceUseType::CRACK), person->isSubstanceUser(SubstanceUseType::ECSTASY));
    stats->personDataRecorder()->recordPREPStart(person, tick);
    runner.scheduleEvent(stop_time, Schedule::FunctorPtr(new PrepCessationEvent(person, stop_time)));
}

PrepInterventionManager::PrepInterventionManager() : interventions() {

}

PrepInterventionManager::~PrepInterventionManager() {}

void PrepInterventionManager::addIntervention(std::shared_ptr<IPrepIntervention> intervention) {
    interventions.push_back(intervention);
}

void PrepInterventionManager::processPerson(std::shared_ptr<Person>& person, Network<Person>& network) {
    for (auto& intervention : interventions) {
        intervention->processPerson(person, network);
    }
}

void PrepInterventionManager::run(double tick, Network<Person>& network) {
    std::vector<PersonPtr> persons_put_on_prep;
    for (auto& intervention : interventions) {
        intervention->run(tick, persons_put_on_prep, network);
    }
}

void PrepInterventionManager::onYearEnded() {
    for (auto& intervention : interventions) {
        intervention->onYearEnded();
    }
}

void PrepInterventionManager::reset() {
    for (auto& intervention : interventions) {
        intervention->reset();
    }
}

void PrepInterventionManager::clear() {
    interventions.clear();
}

ScheduleIntervention::ScheduleIntervention(PrepInterventionManager* prep_manager) : prep_manager_{prep_manager} {}

ScheduleIntervention::~ScheduleIntervention() {}

void ScheduleIntervention::addIntervention(std::shared_ptr<IPrepIntervention> intervention) {
    interventions.push_back(intervention);
}

void ScheduleIntervention::operator()() {
    std::cout << "Updating intervention" << std::endl;
    prep_manager_->clear();
    for (auto& intervention : interventions) {
        prep_manager_->addIntervention(intervention);
    }
}

}
