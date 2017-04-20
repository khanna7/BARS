/*
 * PersonDataRecorder.cpp
 *
 *  Created on: May 12, 2016
 *      Author: nick
 */

#include "PersonDataRecorder.h"

namespace TransModel {

const std::string PersonData::header("id,time_of_entry,time_of_death,infection_status,time_of_infection,"
		"art_status,time_of_art_initiation,time_of_art_cessation,prep_status,time_of_prep_initiation,"
		"time_of_prep_cessation,number_of_tests,time_since_last_test,diagnosis_status,init_art_lag,adherence_category,"
		"adhered_interval_count,non_adhered_interval_count,infection_source");

PersonData::PersonData(PersonPtr p, double time_of_birth) :
		id_(p->id()), birth_ts(time_of_birth), death_ts(-1), infection_ts(
				p->isInfected() ? p->infectionParameters().time_of_infection : -1), art_init_ts(
				p->isOnART() ? p->infectionParameters().time_of_art_init : -1), art_stop_ts(-1), prep_init_ts(
				p->isOnPrep() ? -1 : -1), prep_stop_ts(-1), prep_status(p->prepStatus()), infection_status(
				p->isInfected()), art_status(p->isOnART()), diagnosed(p->isDiagnosed()), number_of_tests(
				p->diagnoser().testCount()), time_since_last_test { -1 }, adherence_category(static_cast<int>(AdherenceCategory::NA)),
				adhered_interval_count(0), non_adhered_interval_count(0), init_art_lag(-1), infection_source(static_cast<unsigned int>(InfectionSource::NONE)) {
}

void PersonData::writeTo(FileOutput& out) {
	out << id_ << "," << birth_ts << "," << death_ts << "," << infection_status << "," << infection_ts << "," << art_status
			<< "," << art_init_ts << "," << art_stop_ts << "," << static_cast<int>(prep_status) << "," << prep_init_ts << "," << prep_stop_ts
			<< "," << number_of_tests << "," << time_since_last_test << "," << diagnosed << "," << init_art_lag << "," << adherence_category << "," <<
			adhered_interval_count << "," << non_adhered_interval_count << "," << infection_source << "\n";
}


PersonDataRecorder::PersonDataRecorder(const std::string& fname, unsigned int buffer) : data{},
		writer{fname, PersonData::header, buffer} {}

PersonDataRecorder::~PersonDataRecorder() {
	for (auto& item : data) {
		writer.addOutput(item.second);
	}
}

void PersonDataRecorder::recordInitialARTLag(PersonPtr& p, double lag) {
	data.at(p->id()).init_art_lag = lag;
}

void PersonDataRecorder::recordARTStart(PersonPtr& p, double ts) {
	data.at(p->id()).art_status = true;
	data.at(p->id()).art_init_ts = ts;
}

void PersonDataRecorder::recordARTStop(PersonPtr& p, double ts) {
	data.at(p->id()).art_status = false;
	data.at(p->id()).art_stop_ts = ts;
}

void PersonDataRecorder::recordPREPStart(int id, double ts) {
	data.at(id).prep_status = PrepStatus::ON;
	data.at(id).prep_init_ts = ts;
}

void PersonDataRecorder::recordPREPStop(int id, double ts, PrepStatus status) {
	data.at(id).prep_status = status;
	data.at(id).prep_stop_ts = ts;
}

void PersonDataRecorder::recordInfection(PersonPtr& p, double ts, InfectionSource source) {
	PersonData& pd = data.at(p->id());
	pd.infection_status = true;
	pd.infection_ts = ts;
	pd.infection_source = static_cast<unsigned int>(source);
}

void PersonDataRecorder::finalize(const PersonPtr& p, double ts) {
	PersonData& pd = data.at(p->id());
	pd.number_of_tests = p->diagnoser().testCount();
	double lt =  p->diagnoser().lastTestAt();
	pd.time_since_last_test = lt == -1.0 ? -1.0 : ts - lt;
	pd.diagnosed = p->isDiagnosed();
	pd.prep_status = p->prepStatus();
	pd.adherence_category = static_cast<int>(p->adherence().category);
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

void PersonDataRecorder::incrementNonAdheredIntervals(PersonPtr& p) {
	PersonData& pd = data.at(p->id());
	++pd.non_adhered_interval_count;
}

void PersonDataRecorder::incrementAdheredIntervals(PersonPtr& p) {
	PersonData& pd = data.at(p->id());
	++pd.adhered_interval_count;
}

} /* namespace TransModel */
