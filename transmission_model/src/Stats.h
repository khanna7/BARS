/*
 * Stats.h
 *
 *  Created on: Nov 6, 2015
 *      Author: nick
 */

#ifndef SRC_STATS_H_
#define SRC_STATS_H_

#include <vector>

namespace TransModel {

struct  StatItem {
	unsigned int edge_count, size, infected, total_infected, births, age_deaths, gr_deaths;
};

class Stats {

private:
	std::vector<StatItem> items;
	StatItem current_item;

public:
	Stats();
	virtual ~Stats();

	void incrementCurrentEdgeCount(unsigned int edge_count);
	void incrementCurrentSize(unsigned int edge_count);
	void incrementCurrentTransmissionInfectedCount(unsigned int edge_count);
	void incrementCurrentTotalInfectedCount(unsigned int infected);
	void incrementCurrentBirthCount(unsigned int count);
	void incrementCurrentOADeathCount(unsigned int count);
	void incrementCurrentGRDeathCount(unsigned int count);

	void resetForNextTimeStep();
	void writeToCSV(const std::string& file);
};

} /* namespace TransModel */

#endif /* SRC_STATS_H_ */
