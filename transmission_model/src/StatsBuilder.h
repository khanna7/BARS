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

public:
	StatsBuilder();
	virtual ~StatsBuilder();

	StatsBuilder* countsWriter(const std::string& fname, unsigned int buffer = 1000);
	StatsBuilder* partnershipEventWriter(const std::string& fname, unsigned int buffer = 1000);

	void createStatsSingleton();
};

} /* namespace TransModel */

#endif /* SRC_STATSBUILDER_H_ */
