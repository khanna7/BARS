/*
 * Model.h
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#ifndef SRC_MODEL_H_
#define SRC_MODEL_H_

#include <memory>
#include <vector>
#include <map>

#include "RInside.h"

#include "Person.h"
#include "Network.h"
#include "Stage.h"
#include "TransmissionRunner.h"
#include "CD4Calculator.h"
#include "ViralLoadCalculator.h"
#include "ViralLoadSlopeCalculator.h"
#include "PersonCreator.h"
//#include "DayRangeCalculator.h"
#include "ARTScheduler.h"
#include "CondomUseAssigner.h"
#include "RangeWithProbability.h"
#include "ARTLagCalculator.h"
#include "PrepInterventionManager.h"
#include "Jail.h"
#include "Stats.h"

namespace TransModel {

struct TransmissionParameters {
    double prop_steady_sex_acts, prop_casual_sex_acts;
};

enum class CauseOfDeath {
    NONE, AGE, INFECTION, ASM, ASM_CD4
};

class Model {

private:
    std::shared_ptr<RInside> R;
    Network<Person> net;
    std::vector<PersonPtr> population;
    std::shared_ptr<TransmissionRunner> trans_runner;
    CD4Calculator cd4_calculator;
    ViralLoadCalculator viral_load_calculator;
    ViralLoadSlopeCalculator viral_load_slope_calculator;
    unsigned int current_pop_size, previous_pop_size;
    std::map<float, std::shared_ptr<Stage>> stage_map;
    std::set<int> persons_to_log;
    TransmissionParameters trans_params;
    ARTLagCalculator art_lag_calculator;
    PersonCreator person_creator;
    PrepInterventionManager prep_manager;
    CondomUseAssigner condom_assigner;
    RangeWithProbability asm_runner, cd4m_treated_runner;
    float age_threshold;
    Jail jail;
    unsigned int total_infected_person_days;

    // returns the number infected
    unsigned int runTransmission(double timestamp);
    CauseOfDeath dead(double tick, PersonPtr person, int max_survival);
    void entries(double tick, float size_of_time_step);
    void deactivateEdges(int id, double time);

    /**
     * @param uninfected empty vector into which the uninfected are placed
     */
    void updateVitals(double time, float size_of_time_step, int max_survival, std::vector<PersonPtr>& uninfected);
    void updateDisease(PersonPtr person);
    unsigned int runExternalInfections(std::vector<PersonPtr>& uninfected, double time);

    void infectPerson(PersonPtr& person, double time_stamp);
    void updateThetaForm(const std::string& var_name);
    void countOverlap();

    bool hasSex(int type);
    void schedulePostDiagnosisART(PersonPtr person, std::map<double, ARTScheduler*>& art_map, double tick,
            float size_of_timestep);

    /**
     * Initializes PrEP cessation events for the initial set of persons.
     */
    void initPrepCessation();
    void jailPerson(double tick, PersonPtr& person);

    float incarcerationRate();

     /**
     * These are used for calcualting (infection) incidence:
     */
    int infectedPopulationSize();
    int uninfectedPopulationSize();
    float infectionIncidence();
    float infectionIncidence_personDay(double time);

    void updateJailStats(Stats* stats, PersonPtr person);
    void doJailCheck(PersonPtr person, double, double, double);
    void runJailInfections(double tick);

public:
    Model(std::shared_ptr<RInside>& r_ptr, const std::string& net_var, const std::string& cas_net_var);
    virtual ~Model();

    void step();
    void atEnd();
    void saveRNetwork();
};

} /* namespace TransModel */

#endif /* SRC_MODEL_H_ */
