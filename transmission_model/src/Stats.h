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

namespace TransModel {

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
	static const std::string AGE, INFECTION;

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

	enum PEventType {ENDED, STARTED};

	double tick_;
	int p1_id, p2_id;
	PEventType type_;
	int network_type;

	PartnershipEvent(double tick, int p1, int p2, PEventType type, int net_type);
	void writeTo(FileOutput& out);

};

struct Counts {

	static const std::string header;

	double tick;
	unsigned int main_edge_count, casual_edge_count, size, infected, infected_at_entry, uninfected,
	entries, age_deaths, infection_deaths;
	int overlaps;

	Counts();
	void reset();
	void writeTo(FileOutput& out);

};

class Stats {

private:
	std::shared_ptr<StatsWriter<Counts>> counts_writer;
	Counts current_counts;
	std::shared_ptr<StatsWriter<PartnershipEvent>> pevent_writer;
	std::shared_ptr<StatsWriter<InfectionEvent>> ievent_writer;
	std::shared_ptr<StatsWriter<Biomarker>> biomarker_writer;
	std::shared_ptr<StatsWriter<DeathEvent>> death_writer;

	PersonDataRecorder pd_recorder;

	friend class StatsBuilder;
	static Stats* instance_;

	Stats(std::shared_ptr<StatsWriter<Counts>> counts, std::shared_ptr<StatsWriter<PartnershipEvent>> pevents,
			std::shared_ptr<StatsWriter<InfectionEvent>> infection_event_writer, std::shared_ptr<StatsWriter<Biomarker>> bio_writer,
			std::shared_ptr<StatsWriter<DeathEvent>> death_event_writer, const std::string& person_data_fname);

public:
	virtual ~Stats();

	void resetForNextTimeStep();

	Counts& currentCounts() {
		return current_counts;
	}

	PersonDataRecorder& personDataRecorder() {
		return pd_recorder;
	}

	static Stats* instance() {
		return instance_;
	}

	void recordPartnershipEvent(double time, int p1, int p2, PartnershipEvent::PEventType event_type, int net_type);
	void recordInfectionEvent(double time, const PersonPtr& p1, const PersonPtr& p2, bool condom, int net_type);

	/**
	 * Records an infection event for persons entering the model as infected.
	 */
	void recordInfectionEvent(double time, const PersonPtr& p);
	void recordBiomarker(double time, const PersonPtr& person);
	void recordDeathEvent(double time, const PersonPtr& person, const std::string& cause);
};

} /* namespace TransModel */

#endif /* SRC_STATS_H_ */
