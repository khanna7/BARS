/*
 * Stats.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: nick
 */

#include "boost/filesystem.hpp"

#include "Stats.h"
#include "file_utils.h"

namespace TransModel {

Stats::Stats(const std::string& fname, size_t buffer_size) :
		items(), current_item { 0, 0, 0, 0, 0, 0, 0}, buffer_size_(buffer_size), out {}, last_written_timestep{0} {
	std::string file_name = unique_file_name(fname);
	boost::filesystem::path filepath(file_name);
	if (!boost::filesystem::exists(filepath.parent_path()))
		boost::filesystem::create_directories(filepath.parent_path());

	out.open(file_name.c_str());
	out << "\"time\",\"entries\",\"old_age_deaths\",\"infection_deaths\",\"infected_via_transmission\","
			<< "\"edge_count\",\"vertex_count\"\n";
}

Stats::~Stats() {
	write();
	out.close();
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

void Stats::resetForNextTimeStep() {
	items.push_back(current_item);
	current_item = {0, 0, 0, 0, 0, 0, 0};

	if (items.size() == buffer_size_) {
		write();
	}
}

void Stats::incrementCurrentEntryCount(unsigned int count) {
	current_item.entries += count;
}

void Stats::incrementCurrentOADeathCount(unsigned int count) {
	current_item.age_deaths += count;
}

void Stats::incrementCurrentGRDeathCount(unsigned int count) {
	current_item.gr_deaths += count;
}

//void Stats::incrementCurrentARTCount(unsigned int count) {
//	current_item.on_art += count;
//}

void Stats::write() {
	for (auto item : items) {
		out << last_written_timestep << "," << item.entries << "," << item.age_deaths << "," << item.gr_deaths << "," << item.infected
				<< "," << item.edge_count << "," << item.size << "\n";
		++last_written_timestep;
	}
	items.clear();
}

} /* namespace TransModel */
