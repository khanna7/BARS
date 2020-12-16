/*
 * StatsBuilder.cpp
 *
 *  Created on: Mar 21, 2016
 *      Author: nick
 */

#include "StatsBuilder.h"
#include "Range.h"

namespace TransModel {

StatsBuilder::StatsBuilder(const std::string& out_dir) :
        counts_writer { nullptr }, pevent_writer { nullptr }, ievent_writer(nullptr), biomarker_writer { nullptr }, pd_fname { }, tevent_writer {
                nullptr }, art_event_writer { nullptr }, prep_event_writer { nullptr }, out_dir_ { out_dir } {
}

StatsBuilder::~StatsBuilder() {
}

StatsBuilder* StatsBuilder::artEventWriter(const std::string& fname, unsigned int buffer) {
    if (fname == "") {
        art_event_writer = std::make_shared<NullStatsWriter<ARTEvent>>();
    } else {
        art_event_writer = std::make_shared<StatsWriter<ARTEvent>>(out_dir_ + "/" + fname, ARTEvent::header, buffer);
    }
    return this;
}

StatsBuilder* StatsBuilder::prepEventWriter(const std::string& fname, unsigned int buffer) {
    if (fname == "") {
        prep_event_writer = std::make_shared<NullStatsWriter<PREPEvent>>();
    } else {
        prep_event_writer = std::make_shared<StatsWriter<PREPEvent>>(out_dir_ + "/" + fname, PREPEvent::header, buffer);
    }
    return this;
}

StatsBuilder* StatsBuilder::countsWriter(const std::string& fname, unsigned int buffer) {
    if (fname == "") {
        counts_writer = std::make_shared<NullStatsWriter<Counts>>();
    } else {
        counts_writer = std::make_shared<StatsWriter<Counts>>(out_dir_ + "/" + fname, Counts::header, buffer);
    }
    return this;
}

StatsBuilder* StatsBuilder::partnershipEventWriter(const std::string& fname, unsigned int buffer) {
    if (fname == "") {
        pevent_writer = std::make_shared<NullStatsWriter<PartnershipEvent>>();
    } else {
        pevent_writer = std::make_shared<StatsWriter<PartnershipEvent>>(out_dir_ + "/" + fname,
                PartnershipEvent::header, buffer);
    }
    return this;
}

StatsBuilder* StatsBuilder::infectionEventWriter(const std::string& fname, unsigned int buffer) {
    if (fname == "") {
        ievent_writer = std::make_shared<NullStatsWriter<InfectionEvent>>();
    } else {
        ievent_writer = std::make_shared<StatsWriter<InfectionEvent>>(out_dir_ + "/" + fname, InfectionEvent::header,
                buffer);
    }
    return this;
}

StatsBuilder* StatsBuilder::biomarkerWriter(const std::string& fname, unsigned int buffer) {
    if (fname == "") {
        biomarker_writer = std::make_shared<NullStatsWriter<Biomarker>>();
    } else {
        biomarker_writer = std::make_shared<StatsWriter<Biomarker>>(out_dir_ + "/" + fname, Biomarker::header, buffer);
    }
    return this;
}

StatsBuilder* StatsBuilder::deathEventWriter(const std::string& fname, unsigned int buffer) {
    if (fname == "") {
        death_writer = std::make_shared<NullStatsWriter<DeathEvent>>();
    } else {
        death_writer = std::make_shared<StatsWriter<DeathEvent>>(out_dir_ + "/" + fname, DeathEvent::header, buffer);
    }
    return this;
}

StatsBuilder* StatsBuilder::testingEventWriter(const std::string& fname, unsigned int buffer) {
    if (fname == "") {
        tevent_writer = std::make_shared<NullStatsWriter<TestingEvent>>();
    } else {
        tevent_writer = std::make_shared<StatsWriter<TestingEvent>>(out_dir_ + "/" + fname, TestingEvent::header,
                buffer);
    }
    return this;
}

StatsBuilder* StatsBuilder::personDataRecorder(const std::string& fname) {
    if (fname == "") {
        pd_fname = "";
    } else {
        pd_fname = out_dir_ + "/" + fname;
    }
    return this;
}

StatsBuilder* StatsBuilder::viralLoadEventWriter(const std::string& fname, unsigned int buffer) {
    if (fname == "") {
        viral_load_event_writer = std::make_shared<NullStatsWriter<ViralLoadEvent>>();
    } else {
        viral_load_event_writer = std::make_shared<StatsWriter<ViralLoadEvent>>(out_dir_ + "/" + fname, ViralLoadEvent::header,
                buffer);
    }
    return this;
}

void StatsBuilder::createStatsSingleton(int min_age, int max_age) {
    if (counts_writer && pevent_writer && ievent_writer && biomarker_writer && death_writer && tevent_writer
            && art_event_writer && prep_event_writer && viral_load_event_writer) {
        if (Stats::instance_ != nullptr) {
            delete Stats::instance_;
        }
        Stats::instance_ = new Stats(counts_writer, pevent_writer, ievent_writer, biomarker_writer, death_writer,
                pd_fname, tevent_writer, art_event_writer, prep_event_writer, viral_load_event_writer, min_age, max_age);
    } else {
        throw std::domain_error("Stats must be fully initialized from StatsBuilder before being used.");
    }
}

} /* namespace TransModel */
