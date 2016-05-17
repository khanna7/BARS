/*
 * PersonDataRecorder.cpp
 *
 *  Created on: May 12, 2016
 *      Author: nick
 */

#include "PersonDataRecorder.h"

namespace TransModel {

const std::string PersonData::header("\"id\",\"time of entry\",\"time of death\",\"infection status\",\"time of infection\","
		"\"art status\",\"time of art initiation\",\"time of art cessation\",\"prep status\",\"time of prep initiation\","
		"\"time of prep cessation\"");

PersonData::PersonData(PersonPtr p, double time_of_birth) :
		id_(p->id()), birth_ts(time_of_birth), death_ts(-1), infection_ts(p->isInfected() ? p->infectionParameters().time_of_infection : -1),
		art_init_ts(p->isOnART() ? p->infectionParameters().time_of_art_init : -1),
		art_stop_ts(-1),
		prep_init_ts(p->isOnPrep() ? -1 : -1),
		prep_stop_ts(-1),
		infection_status(p->isInfected()), art_status(p->isOnART()), prep_status(p->isOnPrep()) {
}

void PersonData::writeTo(FileOutput& out) {
	out << id_ << "," << birth_ts << "," << death_ts << "," << infection_status << "," << infection_ts << "," << art_status
			<< "," << art_init_ts << "," << art_stop_ts << "," << prep_status << "," << prep_init_ts << "," << prep_stop_ts << "\n";
}


PersonDataRecorder::PersonDataRecorder(const std::string& fname, unsigned int buffer) : data{},
		writer{fname, PersonData::header, buffer} {}

PersonDataRecorder::~PersonDataRecorder() {
	for (auto& item : data) {
		writer.addOutput(item.second);
	}
}

void PersonDataRecorder::recordARTInit(PersonPtr& p, double ts) {
	data.at(p->id()).art_init_ts = ts;
}

void PersonDataRecorder::recordARTStop(PersonPtr& p, double ts) {
	data.at(p->id()).art_stop_ts = ts;
}

void PersonDataRecorder::recordInfection(PersonPtr& p, double ts) {
	PersonData& pd = data.at(p->id());
	pd.infection_status = true;
	pd.infection_ts = ts;
}

void PersonDataRecorder::recordDeath(PersonPtr& p, double ts) {
	PersonData& pd = data.at(p->id());
	pd.death_ts = ts;
	writer.addOutput(pd);
	data.erase(p->id());
}

void PersonDataRecorder::initRecord(PersonPtr& person, double time_of_entry) {
	PersonData pd(person, time_of_entry);
	data.emplace(person->id(), pd);
}

} /* namespace TransModel */
