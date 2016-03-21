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

std::string PartnershipEvent::header("\"tick\",\"p1\",\"p2\",\"type\"");

PartnershipEvent::PartnershipEvent(double tick, int p1, int p2, PEventType type) : tick_(tick), p1_id(p1), p2_id(p2), type_{type} {}

void PartnershipEvent::writeTo(FileOutput& out) {
	out << tick_ << "," << p1_id << "," << p2_id << "," << static_cast<int>(type_) << "\n";
}


std::string Counts::header("\"time\",\"entries\",\"old_age_deaths\",\"infection_deaths\",\"infected_via_transmission\",\"edge_count\",\"vertex_count\"");

void Counts::writeTo(FileOutput& out) {
	out << tick << "," << entries << "," << age_deaths << "," << gr_deaths << "," << infected
			<< "," << edge_count << "," << size << "\n";
}

Counts::Counts() : tick{0}, edge_count{0}, size{0}, infected{0}, entries{0},
		age_deaths{0}, gr_deaths{0} {}

void Counts::reset() {
	tick = 0;
	edge_count = size = infected = entries = age_deaths = gr_deaths = 0;
}

Stats* Stats::instance_ = nullptr;

Stats::Stats(std::shared_ptr<StatsWriter<Counts>> counts, std::shared_ptr<StatsWriter<PartnershipEvent>> pevents) :
		counts_writer{counts}, current_counts{}, pevent_writer{pevents} {
}

Stats::~Stats() {
}


void Stats::resetForNextTimeStep() {
	counts_writer->addOutput(current_counts);
	current_counts.reset();
}

void Stats::recordPartnershipEvent(double t, int p1, int p2, PartnershipEvent::PEventType event_type) {
	pevent_writer->addOutput(PartnershipEvent{t, p1, p2, event_type});
}



} /* namespace TransModel */
