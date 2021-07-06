/*
 * PersonDataRecorder.h
 *
 *  Created on: May 12, 2016
 *      Author: nick
 */

#ifndef SRC_PERSONDATARECORDER_H_
#define SRC_PERSONDATARECORDER_H_

#include <map>

#include "StatsWriter.h"
#include "Person.h"
#include "common.h"

namespace TransModel {

enum class InfectionSource {INTERNAL, EXTERNAL, INJAIL, NONE};

struct PersonData {

    static const std::string header;

    int id_;
    double birth_ts, death_ts, infection_ts;
    double art_init_ts, art_stop_ts, prep_init_ts, prep_stop_ts;
    PrepStatus prep_status;
    bool infection_status, art_status, diagnosed;
    unsigned int number_of_tests;
    double time_since_last_test;
    int art_adherence_category, prep_adherence_category;
    unsigned int adhered_interval_count, non_adhered_interval_count;
    double init_art_lag;
    unsigned int infection_source;
    double time_of_diagnosis;
    double cd4_at_diagnosis;

    PersonData(PersonPtr p, double time_of_birth);
    void writeTo(FileOutput& out);

};

class PersonDataRecorderI {

public:
    PersonDataRecorderI() {}
    virtual ~PersonDataRecorderI()  {}

    virtual void initRecord(PersonPtr& person, double time_of_entry) = 0;
    virtual void recordARTStart(const PersonPtr& p, double ts) = 0;
    virtual void recordARTStop(const PersonPtr& p, double ts) = 0;
    virtual void recordPREPStart(const PersonPtr& p, double ts) = 0;
    virtual void recordPREPStop(const Person* p, double ts, PrepStatus status) = 0;
    virtual void recordDiagnosis(const Person* p, double ts) = 0;
    virtual void recordInfection(const PersonPtr& p, double ts, InfectionSource source) = 0;
    virtual void recordDeath(const PersonPtr& p, double ts) = 0;
    virtual void recordInitialARTLag(const PersonPtr& p, double lag) = 0;
    virtual void incrementNonAdheredIntervals(const PersonPtr& p) = 0;
    virtual void incrementAdheredIntervals(const PersonPtr& p) = 0;
    virtual void finalize(const PersonPtr& p, double ts) = 0;
};

class PersonDataRecorder : public PersonDataRecorderI {

    std::map<int, PersonData> data;
    StatsWriter<PersonData> writer;

public:
    PersonDataRecorder(const std::string& fname, unsigned int buffer);

    virtual ~PersonDataRecorder();

    void initRecord(PersonPtr& person, double time_of_entry) override;
    void recordARTStart(const PersonPtr& p, double ts) override;
    void recordARTStop(const PersonPtr& p, double ts) override;
    void recordPREPStart(const PersonPtr& p, double ts) override;
    void recordPREPStop(const Person* p, double ts, PrepStatus status) override;
    void recordDiagnosis(const Person* p, double ts) override;
    void recordInfection(const PersonPtr& p, double ts, InfectionSource source) override;
    void recordDeath(const PersonPtr& p, double ts) override;
    void recordInitialARTLag(const PersonPtr& p, double lag) override;
    void incrementNonAdheredIntervals(const PersonPtr& p) override;
    void incrementAdheredIntervals(const PersonPtr& p) override;
    void finalize(const PersonPtr& p, double ts) override;
};

class NullPersonDataRecorder : public PersonDataRecorderI {

public:
    NullPersonDataRecorder() {}

    virtual ~NullPersonDataRecorder() {}

    void initRecord(PersonPtr& person, double time_of_entry) override {}
    void recordARTStart(const PersonPtr& p, double ts) override {}
    void recordARTStop(const PersonPtr& p, double ts) override {}
    void recordPREPStart(const PersonPtr& p, double ts) override {}
    void recordPREPStop(const Person* p, double ts, PrepStatus status) override {}
    void recordInfection(const PersonPtr& p, double ts, InfectionSource source) override {}
    void recordDiagnosis(const Person* p, double ts) override {}
    void recordDeath(const PersonPtr& p, double ts) override {}
    void recordInitialARTLag(const PersonPtr& p, double lag) override {}
    void incrementNonAdheredIntervals(const PersonPtr& p) override {}
    void incrementAdheredIntervals(const PersonPtr& p) override {}
    void finalize(const PersonPtr& p, double ts) override {}
};

} /* namespace TransModel */

#endif /* SRC_PERSONDATARECORDER_H_ */
