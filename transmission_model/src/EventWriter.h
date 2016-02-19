/*
 * EventWriter.h
 *
 *  Created on: Feb 18, 2016
 *      Author: nick
 */

#ifndef SRC_EVENTWRITER_H_
#define SRC_EVENTWRITER_H_

#include <vector>

#include "H5Cpp.h"

#include "common.h"

namespace TransModel {

// TODO create different event structs and an EventDataSet that wraps a
// hdf5 DataSet for each different kind of event

struct Event {

	double tick;
	int person_id;
	float time_of_infection;
	float time_of_art_init;
	const char* event_type;

};

class EventWriter {

private:
	static EventWriter* instance_;

	H5::H5File* file;
	H5::DataSet* dataset;
	H5::CompType event_type;
	unsigned int buffer_size_;
	std::vector<Event> events;
	hsize_t offset;

	void writeEvents();

	EventWriter(const std::string& fname, unsigned int buffer_size);

public:
	~EventWriter();
	static void initialize(const std::string& file_name, unsigned int buffer_size);
	static EventWriter* instance();

	void addEvent(double t, const PersonPtr& person, const std::string & event);

};


} /* namespace TransModel */

#endif /* SRC_EVENTWRITER_H_ */
