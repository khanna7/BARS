/*
 * Stats.h
 *
 *  Created on: Nov 6, 2015
 *      Author: nick
 */

#ifndef SRC_STATS_H_
#define SRC_STATS_H_

#include <vector>
#include <fstream>

#include "FileOutput.h"
#include "StatsWriter.h"
#include "common.h"
#include "PersonDataRecorder.h"
#include "Range.h"

namespace TransModel {

struct ARTEvent {

    static const std::string header;

    double tick;
    int p_id;
    // false is off art, true is on art
    bool type;

    void writeTo(FileOutput& out);
};

struct PREPEvent {

    static const std::string header;

    double tick;
    int p_id;
    // 0 is off prep, 1 is off because infected, 2 is on
    int type;

    void writeTo(FileOutput& out);
};

struct TestingEvent {

    static const std::string header;

    double tick;
    int p_id;
    bool result;

    void writeTo(FileOutput& out);
};

struct Biomarker {

    static const std::string header;

    double tick;
    int p_id;
    float viral_load, cd4;
    bool on_art;

    void writeTo(FileOutput& out);
};

struct DeathEvent {

    static const std::string header;
    static const std::string AGE, INFECTION, ASM, ASM_CD4;

    double tick;
    int p_id;
    float age;
    bool art_status;
    std::string cause;

    void writeTo(FileOutput& out);
};

struct InfectionEvent {

    static const std::string header;

    double tick;
    int p1_id, p2_id;
    float p1_age, p2_age;
    float p1_viral_load, p2_viral_load, p1_infectivity, p1_cd4, p2_cd4;
    bool p1_art, p1_on_prep, p2_on_prep, condom_used;
    int network_type;

    void writeTo(FileOutput& out);
};

struct PartnershipEvent {

    static const std::string header;

    enum PEventType {ENDED_DISSOLUTION, STARTED, ENDED_DEATH_INFECTION, ENDED_DEATH_ASM, ENDED_AGING_OUT, ENDED_DEATH_ASM_CD4};

    double tick_;
    unsigned int edge_id_;
    int p1_id, p2_id;
    PEventType type_;
    int network_type;

    PartnershipEvent(double tick, unsigned int edge_id, int p1, int p2, PEventType type, int net_type);
    void writeTo(FileOutput& out);

};

struct Counts {

    static const std::string header;

    double tick;
    unsigned int main_edge_count, casual_edge_count,
    //size, internal_infected, external_infected, infected_at_entry, uninfected,
    entries, age_deaths, infection_deaths, asm_deaths;
    int overlaps;
    unsigned int sex_acts, casual_sex_acts, steady_sex_acts;
    unsigned int sd_casual_sex_with_condom, sd_casual_sex_without_condom;
    unsigned int sd_steady_sex_with_condom, sd_steady_sex_without_condom;
    unsigned int sc_casual_sex_with_condom, sc_casual_sex_without_condom;
    unsigned int sc_steady_sex_with_condom, sc_steady_sex_without_condom;
    unsigned int on_art, on_prep;
    unsigned int went_on_prep, went_off_prep;
    //unsigned int uninfected_u26, uninfected_gte26, infected_via_transmission_u26, infected_via_transmission_gte26,
    //	vertex_count_u26, vertex_count_gte26;
    //unsigned int external_infected_u26, external_infected_gte26, infected_at_entry_u26, infected_at_entry_gte26;
    // internal_infected is infected by transmission
    std::vector<unsigned int> uninfected, internal_infected, external_infected, infected_at_entry, vertex_count;
    int min_age_;
    double vl_supp_per_positives, vl_supp_per_diagnosis, cd4m_deaths;

    Counts(int min_age, int max_age);
    void reset();
    void writeTo(FileOutput& out);
    void incrementInfected(PersonPtr& p);
    void incrementInfectedAtEntry(PersonPtr& p);
    void incrementInfectedExternal(PersonPtr& p);
    void incrementUninfected(PersonPtr& p);
    void incrementVertexCount(PersonPtr p);
};

class Stats {

private:
    std::shared_ptr<StatsWriterI<Counts>> counts_writer;
    Counts current_counts;
    std::shared_ptr<StatsWriterI<PartnershipEvent>> pevent_writer;
    std::shared_ptr<StatsWriterI<InfectionEvent>> ievent_writer;
    std::shared_ptr<StatsWriterI<Biomarker>> biomarker_writer;
    std::shared_ptr<StatsWriterI<DeathEvent>> death_writer;
    std::shared_ptr<StatsWriterI<TestingEvent>> tevent_writer;
    std::shared_ptr<StatsWriterI<ARTEvent>> art_event_writer;
    std::shared_ptr<StatsWriterI<PREPEvent>> prep_event_writer;

    std::shared_ptr<PersonDataRecorderI> pd_recorder;

    friend class StatsBuilder;
    static Stats* instance_;

    Stats(std::shared_ptr<StatsWriterI<Counts>> counts, std::shared_ptr<StatsWriterI<PartnershipEvent>> pevents,
            std::shared_ptr<StatsWriterI<InfectionEvent>> infection_event_writer, std::shared_ptr<StatsWriterI<Biomarker>> bio_writer,
            std::shared_ptr<StatsWriterI<DeathEvent>> death_event_writer, const std::string& person_data_fname,
            std::shared_ptr<StatsWriterI<TestingEvent>> testing_event_writer, std::shared_ptr<StatsWriterI<ARTEvent>> art_event_writer,
            std::shared_ptr<StatsWriterI<PREPEvent>> prep_event_writer, int min_age, int max_age);

public:
    virtual ~Stats();

    void resetForNextTimeStep();

    Counts& currentCounts() {
        return current_counts;
    }

    std::shared_ptr<PersonDataRecorderI> personDataRecorder() {
        return pd_recorder;
    }

    static Stats* instance() {
        return instance_;
    }

    void recordPartnershipEvent(double time, unsigned int edge_id, int p1, int p2, PartnershipEvent::PEventType event_type, int net_type);
    void recordInfectionEvent(double time, const PersonPtr& p1, const PersonPtr& p2, bool condom, int net_type);

    /**
     * Records an infection event for persons entering the model as infected.
     */
    void recordInfectionEvent(double time, const PersonPtr& p);
    void recordBiomarker(double time, const PersonPtr& person);
    void recordDeathEvent(double time, const PersonPtr& person, const std::string& cause);
    void recordTestingEvent(double time, int p_id, bool result);
    void recordARTEvent(double time, int p_id, bool onART);
    void recordPREPEvent(double time, int p_id, int type);
};

} /* namespace TransModel */

#endif /* SRC_STATS_H_ */
