/*
 * Stats.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: nick
 */

#include <fstream>
#include "boost/filesystem.hpp"

#include "Stats.h"

namespace TransModel {

Stats::Stats() : items(), current_item { 0, 0, 0, 0, 0, 0, 0} {
}

Stats::~Stats() {
}

void Stats::incrementCurrentEdgeCount(unsigned int edge_count) {
	current_item.edge_count += edge_count;
}
void Stats::incrementCurrentSize(unsigned int size) {
	current_item.size += size;
}
void Stats::incrementCurrentTransmissionInfectedCount(unsigned int infected) {
	current_item.infected += infected;
}

void Stats::incrementCurrentTotalInfectedCount(unsigned int infected) {
	current_item.total_infected += infected;
}

void Stats::resetForNextTimeStep() {
	items.push_back(current_item);
	current_item =  {0, 0, 0, 0, 0, 0, 0};
	current_item.total_infected = items[items.size() - 1].total_infected;
}

void Stats::incrementCurrentBirthCount(unsigned int count) {
	current_item.births += count;
}

void Stats::incrementCurrentOADeathCount(unsigned int count) {
	current_item.age_deaths += count;
}

void Stats::incrementCurrentGRDeathCount(unsigned int count) {
	current_item.gr_deaths += count;
}

void Stats::writeToCSV(const std::string& file) {
	boost::filesystem::path p(file);
	boost::filesystem::create_directories(p.parent_path());
	std::ofstream out;
	out.open(p.c_str());
	out << "\"time\",\"births\",\"old_age_deaths\",\"grim_repear_deaths\",\"total_number_infected\"," <<
			"\"infected_via_transmission\",\"edge_count\",\"vertex_count\"\n";
	int t = 1;
	for (auto item : items) {
		out << t << "," << item.births << "," << item.age_deaths << "," << item.gr_deaths << "," << item.total_infected << "," <<
				item.infected << "," << item.edge_count << "," << item.size << "\n";
		++t;
	}
	out.close();
}

} /* namespace TransModel */
