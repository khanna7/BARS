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
		"\"time of prep cessation\",\"number of tests\",\"time since last test\",\"diagnosis status\"");

PersonData::PersonData(PersonPtr p, double time_of_birth) :
		id_(p->id()), birth_ts(time_of_birth), death_ts(-1), infection_ts(p->isInfected() ? p->infectionParameters().time_of_infection : -1),
		art_init_ts(p->isOnART() ? p->infectionParameters().time_of_art_init : -1),
		art_stop_ts(-1),
		prep_init_ts(p->isOnPrep() ? -1 : -1),
		prep_stop_ts(-1),
		prep_status(p->prepStatus()),
		infection_status(p->isInfected()), art_status(p->isOnART()), diagnosed(p->isDiagnosed()),
				number_of_tests(p->diagnoser().testCount()), time_since_last_test{-1} {
}

void PersonData::writeTo(FileOutput& out) {
	out << id_ << "," << birth_ts << "," << death_ts << "," << infection_status << "," << infection_ts << "," << art_status
			<< "," << art_init_ts << "," << art_stop_ts << "," << static_cast<int>(prep_status) << "," << prep_init_ts << "," << prep_stop_ts
			<< "," << number_of_tests << "," << time_since_last_test << "," << diagnosed << "\n";
}


PersonDataRecorder::PersonDataRecorder(const std::string& fname, unsigned int buffer) : data{},
		writer{fname, PersonData::header, buffer} {}

PersonDataRecorder::~PersonDataRecorder() {
	for (auto& item : data) {
		writer.addOutput(item.second);
	}
}

void PersonDataRecorder::recordARTInit(PersonPtr& p, double ts) {
	data.at(p->id()).art_status = true;
	data.at(p->id()).art_init_ts = ts;
}

void PersonDataRecorder::recordARTStop(PersonPtr& p, double ts) {
	data.at(p->id()).art_status = false;
	data.at(p->id()).art_stop_ts = ts;
}

void PersonDataRecorder::recordInfection(PersonPtr& p, double ts) {
	PersonData& pd = data.at(p->id());
	pd.infection_status = true;
	pd.infection_ts = ts;
}

void PersonDataRecorder::finalize(const PersonPtr& p, double ts) {
	PersonData& pd = data.at(p->id());
	pd.number_of_tests = p->diagnoser().testCount();
	double lt =  p->diagnoser().lastTestAt();
	pd.time_since_last_test = lt == -1.0 ? -1.0 : ts - lt;
	pd.diagnosed = p->isDiagnosed();
	pd.prep_status = p->prepStatus();
}

void PersonDataRecorder::recordDeath(PersonPtr& p, double ts) {
	PersonData& pd = data.at(p->id());
	pd.death_ts = ts;
	finalize(p, ts);
	writer.addOutput(pd);
	data.erase(p->id());
}

void PersonDataRecorder::initRecord(PersonPtr& person, double time_of_entry) {
	PersonData pd(person, time_of_entry);
	data.emplace(person->id(), pd);
}

} /* namespace TransModel */
