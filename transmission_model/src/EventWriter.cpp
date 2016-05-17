/*
 * EventWriter.cpp
 *
 *  Created on: Feb 18, 2016
 *      Author: nick
 */

#include "EventWriter.h"

using namespace H5;

namespace TransModel {

EventWriter* EventWriter::instance_ = 0;


void EventWriter::initialize(const std::string& file_name, unsigned int buffer_size) {
	if (instance_ != 0) {
		delete instance_;
	}
	instance_ = new EventWriter(file_name, buffer_size);
}

EventWriter* EventWriter::instance() {
	if (instance_ == 0)
		throw std::domain_error("EventWriter must be initialized before instance() is called");

	return instance_;
}


EventWriter::EventWriter(const std::string& fname, unsigned int buffer_size) :
		file { nullptr }, dataset { nullptr }, event_type(sizeof(Event)), buffer_size_ { buffer_size }, events { }, offset {
				0 } {

	hsize_t dim[] = { 0 };
	hsize_t maxdims[] = { H5S_UNLIMITED };
	// rank is number of dimensions
	int rank = 1;
	DataSpace space(rank, dim, maxdims);
	// delete the data in the old file or create a new one if it doesn't exist
	file = new H5File(fname, H5F_ACC_TRUNC);

	// enable chunking so dataset will be extendible

	DSetCreatPropList cparms;
	hsize_t chunk_dims[] = { buffer_size_ };
	cparms.setChunk(rank, chunk_dims);
	//cparms.setFillValue(event_type, &fill_event);

	event_type.insertMember("tick", HOFFSET(Event, tick), PredType::NATIVE_DOUBLE);
	event_type.insertMember("id", HOFFSET(Event, person_id), PredType::NATIVE_INT);
	event_type.insertMember("time_of_infection", HOFFSET(Event, time_of_infection), PredType::NATIVE_FLOAT);
	event_type.insertMember("time_of_art_init", HOFFSET(Event, time_of_art_init), PredType::NATIVE_FLOAT);

	StrType str_type(PredType::C_S1);
	str_type.setSize(H5T_VARIABLE);
	event_type.insertMember("event", HOFFSET(Event, event_type), str_type);

	dataset = new DataSet(file->createDataSet("events", event_type, space, cparms));
}

EventWriter::~EventWriter() {
	writeEvents();
	dataset->close();
	file->close();
	delete dataset;
	delete file;
}

void EventWriter::addEvent(double t, const PersonPtr& person, const std::string& event) {
	events.push_back( Event{ t, person->id(),
		person->infectionParameters().time_of_infection,
		person->infectionParameters().time_of_art_init,
		event.c_str() });

	if (events.size() == buffer_size_) {
		writeEvents();
	}
}

void EventWriter::writeEvents() {
	if (events.size() > 0) {
		hsize_t size[1];
		// offset is the count of all the events
		size[0] = events.size() + offset;
		dataset->extend(size);

		DataSpace fspace = dataset->getSpace();
		hsize_t offsets[1];
		offsets[0] = offset;
		offset += events.size();
		hsize_t dims[1];
		dims[0] = events.size();
		fspace.selectHyperslab(H5S_SELECT_SET, dims, offsets);
		DataSpace memspace(1, dims, NULL);
		dataset->write(events.data(), event_type, memspace, fspace);
		events.clear();
	}
}

} /* namespace TransModel */
