/**
 * PrepInterventionManager.cpp
 */

#include "PrepInterventionManager.h"

#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Schedule.h"

#include "Stats.h"
#include "PrepCessationEvent.h"
#include "Parameters.h"

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

PrepIntervention::PrepIntervention(PrepUptakeData& data) : IPrepIntervention(), cessation_generator(data.cessation_stop, 1.1), year(1)
{

}

PrepIntervention::~PrepIntervention() {}

void PrepIntervention::putOnPrep(double tick, std::shared_ptr<Person>& person, PrepStatus cause) {
    ScheduleRunner& runner = RepastProcess::instance()->getScheduleRunner();
    double stop_time;
    if (cause == PrepStatus::ON) {
        double delay = cessation_generator.next();
        stop_time = tick + delay;
    } else if (cause == PrepStatus::ON_LAI) {
        stop_time = tick + Parameters::instance()->getDoubleParameter(LAI_PREP_LAI_LENGTH_DAYS);
    }
    std::cout << stop_time << endl;
    person->goOnPrep(tick, stop_time, cause);
    Stats* stats = Stats::instance();
    stats->recordPREPEvent(tick, person->id(), static_cast<int>(cause));
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
