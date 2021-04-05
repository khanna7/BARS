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
#include "Range.h"

namespace TransModel {

class StatsBuilder {

private:
    std::shared_ptr<StatsWriterI<Counts>> counts_writer;
    std::shared_ptr<StatsWriterI<PartnershipEvent>> pevent_writer;
    std::shared_ptr<StatsWriterI<InfectionEvent>> ievent_writer;
    std::shared_ptr<StatsWriterI<Biomarker>> biomarker_writer;
    std::shared_ptr<StatsWriterI<DeathEvent>> death_writer;
    std::string pd_fname;
    std::shared_ptr<StatsWriterI<TestingEvent>> tevent_writer;
    std::shared_ptr<StatsWriterI<ARTEvent>> art_event_writer;
    std::shared_ptr<StatsWriterI<PREPEvent>> prep_event_writer;
    std::string out_dir_;

public:
    StatsBuilder(const std::string& out_dir);
    virtual ~StatsBuilder();

    StatsBuilder* countsWriter(const std::string& fname, unsigned int buffer = 1);
    StatsBuilder* partnershipEventWriter(const std::string& fname, unsigned int buffer = 1000);
    StatsBuilder* infectionEventWriter(const std::string& fname, unsigned int buffer = 1000);
    StatsBuilder* biomarkerWriter(const std::string& fname, unsigned int buffer = 1000);
    StatsBuilder* deathEventWriter(const std::string& fname, unsigned int buffer = 1000);
    StatsBuilder* testingEventWriter(const std::string& fname, unsigned int buffer = 1000);
    StatsBuilder* personDataRecorder(const std::string& fname);
    StatsBuilder* artEventWriter(const std::string& fname,  unsigned int buffer = 1000);
    StatsBuilder* prepEventWriter(const std::string& fname,  unsigned int buffer = 1000);


    void createStatsSingleton(int min_age, int max_age);
};

} /* namespace TransModel */

#endif /* SRC_STATSBUILDER_H_ */
