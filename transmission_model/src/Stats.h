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

namespace TransModel {

struct  StatItem {
	unsigned int edge_count, size, infected,
	entries, age_deaths, gr_deaths, on_art;
};

class Stats {

private:
	std::vector<StatItem> items;
	StatItem current_item;
	size_t buffer_size_;
	std::ofstream out;
	unsigned int last_written_timestep;

	void write();

public:
	Stats(const std::string& fname, size_t buffer_size = 1000);
	virtual ~Stats();

	void incrementCurrentEdgeCount(unsigned int edge_count);
	void incrementCurrentSize(unsigned int edge_count);
	void incrementCurrentTransmissionInfectedCount(unsigned int edge_count);
	void incrementCurrentEntryCount(unsigned int count);
	void incrementCurrentOADeathCount(unsigned int count);
	void incrementCurrentGRDeathCount(unsigned int count);
	//void incrementCurrentARTCount(unsigned int count);

	void resetForNextTimeStep();
};

} /* namespace TransModel */

#endif /* SRC_STATS_H_ */
