/*
 * Person.h
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#ifndef SRC_PERSON_H_
#define SRC_PERSON_H_

#include "Rcpp.h"
#include "DiseaseParameters.h"
#include "Diagnoser.h"
#include "GeometricDistribution.h"
#include "AdherenceCategory.h"
#include "PrepParameters.h"
#include "JailParameters.h"

namespace TransModel {

class PersonCreator;

class Person {

private:
    friend PersonCreator;

    int id_, steady_role_, casual_role_;
    float age_;
    bool circum_status_;
    InfectionParameters infection_parameters_;
    float infectivity_;
    PrepParameters prep_;
    bool dead_, diagnosed_, testable_;
    Diagnoser diagnoser_;
    AdherenceData art_adherence_;

    double score_;

    JailParameters jail_parameters_;

public:
    Person(int id, float age, bool circum_status, int steady_role, int casual_role,
            Diagnoser diagnoser);

    virtual ~Person();

    /**
     * Gets the id of this person.
     */

    int id() const {
        return id_;
    }

    int steady_role() const {
        return steady_role_;
    }

    int casual_role() const {
        return casual_role_;
    }

    /**
     * Gets the age of this Person.
     */
    float age() const {
        return age_;
    }

    bool isOnPrep() const {
        return prep_.status() == PrepStatus::ON;
    }

    const PrepStatus prepStatus() const {
        return prep_.status();
    }

    const InfectionParameters& infectionParameters() const {
        return infection_parameters_;
    }

    bool isCircumcised() const {
        return circum_status_;
    }

    bool isOnART() const {
        return infection_parameters_.art_status;
    }

    bool isInfected() const {
        return infection_parameters_.infection_status;
    }

    float infectivity() const {
        return infectivity_;
    }

    float timeSinceInfection() const {
        return infection_parameters_.time_since_infection;
    }

    const Diagnoser diagnoser() const {
        return diagnoser_;
    }

    void setArtAdherence(AdherenceData data) {
        art_adherence_ = data;
    }

    const AdherenceData artAdherence() const {
        return art_adherence_;
    }

    double score() const {
        return score_;
    }

    void setScore(double s);

    void setViralLoad(float viral_load);

    void setCD4Count(float cd4_count);

    void setViralLoadARTSlope(float slope);

    void setInfectivity(float infectivity);

    void setAge(float age);

    void goOnPrep(double start_time, double stop_time);

    void goOffPrep(PrepStatus off_status);

    /**
     * Puts this Person on ART with the specified time stamp,
     * setting the art status to true.
     */
    void goOnART(float time_stamp);

    /**
     * Takes this person off ART, setting the art status to false.
     */
    void goOffART();

    /**
     * Infects this Person and sets the duration of the infection,
     * and the time of infection.
     */
    void infect(float duration_of_infection, float time);

    /**
     * Updates age, etc. of person., to be called each iteration of the model.
     *
     * @return true if, in aging, this person has crossed the specified threshold.
     */
    bool step(float size_of_timestep, float threshold);

    /**
     * Checks if person is dead of old age. This doesn't kill
     * the person, it just checks.
     */
    bool deadOfAge(int max_age);

    /**
     * Checks if person is dead of AIDS. This doesn't kill
     * the person, it just checks.
     */
    bool deadOfInfection();

    void setDead(bool isDead) {
        dead_ = isDead;
    }

    bool isDead() const {
        return dead_;
    }

    bool isDiagnosed() const {
        return diagnosed_;
    }

    bool isTestable() const {
        return testable_;
    }

    const PrepParameters prepParameters() const {
        return prep_;
    }

    void setARTLag(double lag) {
        infection_parameters_.art_lag = lag;
    }

    bool diagnose(double tick);

    /**
     * Updates the diagnoser component with new test_prob etc.
     */
    void updateDiagnoser(double test_prob, bool testable);

    /**
     * Updates the prep AdherenceData.
     */
    void updatePrepAdherence(AdherenceData& data);

    /**
     * Boolean function to check if a person is jailed (in jail).
     */
    bool isJailed() const {
        return jail_parameters_.is_in_jail;
    }

    void jailed(double time, double serving_time);

    void releasedFromJail(double release_time); 

    /**
     * Get the time the person was jailed
     */
    float timeOfJail() const {
        return jail_parameters_.time_of_jail;
    }

    /**
     * Get person's jail serving time
     */
    float jailServingTime() const {
        return jail_parameters_.serving_time;
    }


    /**
     * Get the age of the person at the time of first jail (history)
     */
    float ageAtFirstJail() const {
        return jail_parameters_.age_at_first_jail;
    }

        /**
     * Get the age of the person at the time of jail
     */
    float ageAtJail() const {
        return jail_parameters_.age_at_jail;
    }

    /**
     * Get the time since the person was jailed
     */
    float timeSinceJailed(double current_time) const {
        return (current_time - jail_parameters_.time_of_jail);
    }

    /**
     * Debuging helper function: print jail record of a jailed person
     */ 
    void printJailRecord(double current_time)  {
        return jail_parameters_.printJailRecord(current_time);
    } 

};

} /* namespace TransModel */

#endif /* SRC_PERSON_H_ */
