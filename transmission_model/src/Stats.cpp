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

const std::string InfectionEvent::header("\"tick\",\"p1\",\"p1_age\",\"p1_viral_load\",\"p1_cd4\",\"p1_art_status\",\"p1_infectivity\"," \
		"\"condom_used\",\"p2\",\"p2_age\",\"p2_viral_load\",\"p2_cd4\"");

void InfectionEvent::writeTo(FileOutput& out) {
	out << tick << "," << p1_id << "," << p1_age << "," << p1_viral_load << "," <<
			p1_cd4 << "," << p1_art << "," << p1_infectivity << "," << condom_used <<
			"," << p2_id << "," << p2_age << "," << p2_viral_load << "," << p2_cd4 << "\n";
}

const std::string PartnershipEvent::header("\"tick\",\"p1\",\"p2\",\"type\"");

PartnershipEvent::PartnershipEvent(double tick, int p1, int p2, PEventType type) : tick_(tick), p1_id(p1), p2_id(p2), type_{type} {}

void PartnershipEvent::writeTo(FileOutput& out) {
	out << tick_ << "," << p1_id << "," << p2_id << "," << static_cast<int>(type_) << "\n";
}


const std::string Counts::header("\"time\",\"entries\",\"old_age_deaths\",\"infection_deaths\",\"infected_via_transmission\",\"edge_count\",\"vertex_count\"");

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

Stats::Stats(std::shared_ptr<StatsWriter<Counts>> counts, std::shared_ptr<StatsWriter<PartnershipEvent>> pevents,
		std::shared_ptr<StatsWriter<InfectionEvent>> ievent) :
		counts_writer{counts}, current_counts{}, pevent_writer{pevents}, ievent_writer(ievent) {
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

void Stats::recordInfectionEvent(double time, const PersonPtr& p1, const PersonPtr& p2, bool condom) {
	InfectionEvent evt;
	evt.tick = time;
	evt.p1_id = p1->id();
	evt.p1_age = p1->age();
	evt.p1_art = p1->infectionParameters().art_status;
	evt.p1_cd4 = p1->infectionParameters().cd4_count;
	evt.p1_infectivity = p1->infectivity();
	evt.p1_viral_load = p1->infectionParameters().viral_load;
	evt.p2_id = p2->id();
	evt.p2_age = p2->age();
	evt.p2_cd4 = p2->infectionParameters().cd4_count;
	evt.p2_viral_load = p2->infectionParameters().viral_load;
	evt.condom_used = condom;
	ievent_writer->addOutput(evt);
}



} /* namespace TransModel */
