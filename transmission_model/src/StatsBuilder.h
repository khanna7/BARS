/*
 * StatsBuilder.h
 *
 *  Created on: Mar 21, 2016
 *      Author: nick
 */

#ifndef SRC_STATSBUILDER_H_
#define SRC_STATSBUILDER_H_

#include <memory>

#include "StatsWriter.h"
#include "Stats.h"

namespace TransModel {

class StatsBuilder {

private:
	std::shared_ptr<StatsWriter<Counts>> counts_writer;
	std::shared_ptr<StatsWriter<PartnershipEvent>> pevent_writer;
	std::shared_ptr<StatsWriter<InfectionEvent>> ievent_writer;
	std::shared_ptr<StatsWriter<Biomarker>> biomarker_writer;
	std::shared_ptr<StatsWriter<DeathEvent>> death_writer;
	std::string pd_fname;
	std::shared_ptr<StatsWriter<TestingEvent>> tevent_writer;

public:
	StatsBuilder();
	virtual ~StatsBuilder();

	StatsBuilder* countsWriter(const std::string& fname, unsigned int buffer = 1000);
	StatsBuilder* partnershipEventWriter(const std::string& fname, unsigned int buffer = 1000);
	StatsBuilder* infectionEventWriter(const std::string& fname, unsigned int buffer = 1000);
	StatsBuilder* biomarkerWriter(const std::string& fname, unsigned int buffer = 1000);
	StatsBuilder* deathEventWriter(const std::string& fname, unsigned int buffer = 1000);
	StatsBuilder* testingEventWriter(const std::string& fname, unsigned int buffer = 1000);
	StatsBuilder* personDataRecorder(const std::string& fname);


	void createStatsSingleton();
};

} /* namespace TransModel */

#endif /* SRC_STATSBUILDER_H_ */
