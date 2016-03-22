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

namespace TransModel {

struct InfectionEvent {

	static const std::string header;

	double tick;
	int p1_id, p2_id;
	float p1_age, p2_age;
	float p1_viral_load, p2_viral_load, p1_infectivity, p1_cd4, p2_cd4;
	bool p1_art, condom_used;

	void writeTo(FileOutput& out);

};

struct PartnershipEvent {

	static const std::string header;

	enum PEventType {ENDED, STARTED};

	double tick_;
	int p1_id, p2_id;
	PEventType type_;

	PartnershipEvent(double tick, int p1, int p2, PEventType type);
	void writeTo(FileOutput& out);

};

struct Counts {

	static const std::string header;

	double tick;
	unsigned int edge_count, size, infected,
	entries, age_deaths, gr_deaths;

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

	friend class StatsBuilder;
	static Stats* instance_;

	Stats(std::shared_ptr<StatsWriter<Counts>> counts, std::shared_ptr<StatsWriter<PartnershipEvent>> pevents,
			std::shared_ptr<StatsWriter<InfectionEvent>> infection_event_writer);

public:
	virtual ~Stats();

	void resetForNextTimeStep();

	Counts& currentCounts() {
		return current_counts;
	}

	static Stats* instance() {
		return instance_;
	}

	void recordPartnershipEvent(double time, int p1, int p2, PartnershipEvent::PEventType event_type);
	void recordInfectionEvent(double time, const PersonPtr& p1, const PersonPtr& p2, bool condom);
};

} /* namespace TransModel */

#endif /* SRC_STATS_H_ */
