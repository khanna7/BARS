/*
 * PersonDataRecorder.h
 *
 *  Created on: May 12, 2016
 *      Author: nick
 */

#ifndef SRC_PERSONDATARECORDER_H_
#define SRC_PERSONDATARECORDER_H_

#include <map>

#include "StatsWriter.h"
#include "Person.h"
#include "common.h"

namespace TransModel {


struct PersonData {

	static const std::string header;

	int id_;
	double birth_ts, death_ts, infection_ts;
	double art_init_ts, art_stop_ts, prep_init_ts, prep_stop_ts;
	bool infection_status, art_status, prep_status;

	PersonData(PersonPtr p, double time_of_birth);
	void writeTo(FileOutput& out);

};

class PersonDataRecorder {

	std::map<int, PersonData> data;
	StatsWriter<PersonData> writer;

private:

public:
	PersonDataRecorder(const std::string& fname, unsigned int buffer);

	virtual ~PersonDataRecorder();

	void initRecord(PersonPtr& person, double time_of_entry);
	void recordARTInit(PersonPtr& p, double ts);
	void recordARTStop(PersonPtr& p, double ts);
	void recordInfection(PersonPtr& p, double ts);
	void recordDeath(PersonPtr& p, double ts);

};

} /* namespace TransModel */

#endif /* SRC_PERSONDATARECORDER_H_ */
