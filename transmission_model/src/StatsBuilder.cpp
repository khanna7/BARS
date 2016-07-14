/*
 * StatsBuilder.cpp
 *
 *  Created on: Mar 21, 2016
 *      Author: nick
 */

#include "StatsBuilder.h"

namespace TransModel {

StatsBuilder::StatsBuilder(const std::string& out_dir) : counts_writer{nullptr}, pevent_writer{nullptr}, ievent_writer(nullptr),
		biomarker_writer{nullptr}, pd_fname{}, tevent_writer{nullptr}, out_dir_{out_dir} {
}

StatsBuilder::~StatsBuilder() {
}

StatsBuilder* StatsBuilder::countsWriter(const std::string& fname, unsigned int buffer) {
	counts_writer = std::make_shared<StatsWriter<Counts>>(out_dir_ + "/" + fname, Counts::header, buffer);
	return this;

}

StatsBuilder* StatsBuilder::partnershipEventWriter(const std::string& fname, unsigned int buffer) {
	pevent_writer = std::make_shared<StatsWriter<PartnershipEvent>>(out_dir_ + "/" + fname, PartnershipEvent::header, buffer);
	return this;
}

StatsBuilder* StatsBuilder::infectionEventWriter(const std::string& fname, unsigned int buffer) {
	ievent_writer = std::make_shared<StatsWriter<InfectionEvent>>(out_dir_ + "/" + fname, InfectionEvent::header, buffer);
	return this;
}

StatsBuilder* StatsBuilder::biomarkerWriter(const std::string& fname, unsigned int buffer) {
	biomarker_writer = std::make_shared<StatsWriter<Biomarker>>(out_dir_ + "/" + fname, Biomarker::header, buffer);
	return this;
}

StatsBuilder* StatsBuilder::deathEventWriter(const std::string& fname, unsigned int buffer) {
	death_writer = std::make_shared<StatsWriter<DeathEvent>>(out_dir_ + "/" + fname, DeathEvent::header, buffer);
	return this;
}

StatsBuilder* StatsBuilder::testingEventWriter(const std::string& fname, unsigned int buffer) {
	tevent_writer = std::make_shared<StatsWriter<TestingEvent>>(out_dir_ + "/" + fname, TestingEvent::header, buffer);
	return this;
}

StatsBuilder* StatsBuilder::personDataRecorder(const std::string& fname) {
	pd_fname = out_dir_ + "/" + fname;
	return this;
}

void StatsBuilder::createStatsSingleton() {
	if (counts_writer && pevent_writer && ievent_writer && biomarker_writer && death_writer && tevent_writer &&
			pd_fname.length() > 0) {
		if (Stats::instance_ != nullptr) {
			delete Stats::instance_;
		}
		Stats::instance_ = new Stats(counts_writer, pevent_writer, ievent_writer, biomarker_writer,
				death_writer, pd_fname, tevent_writer);
	} else {
		throw std::domain_error("Stats must be fully initialized from StatsBuilder before being used.");
	}
}

} /* namespace TransModel */
