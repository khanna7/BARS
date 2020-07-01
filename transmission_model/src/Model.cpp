/*
 * Model.cpp
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#include <cmath>
#include <exception>

#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"

#include "repast_hpc/Random.h"
#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Utilities.h"

#include "Range.h"
#include "Parameters.h"
#include "Model.h"
#include "network_utils.h"
#include "common.h"
#include "TransmissionRunner.h"
#include "DiseaseParameters.h"
#include "PersonCreator.h"
#include "ARTScheduler.h"
#include "Stats.h"
#include "StatsBuilder.h"
#include "file_utils.h"
#include "utils.h"
#include "CondomUseAssigner.h"
#include "PrepCessationEvent.h"
#include "adherence_functions.h"
#include "BasePrepIntervention.h"
#include "SerodiscordantPrepIntervention.h"
#include "NetStatPrepIntervention.h"
#include "RandomSelectionPrepIntervention.h"
#include "Logger.h"

#include "debug_utils.h"
#include "PrintHelper.h"
#include "PersonToVAL.h"
#include "Serializer.h"

#include "CSVWriter.h"
#include "JailIntervention.h"

//#include "EventWriter.h"

using namespace Rcpp;
using namespace std;
using namespace repast;

namespace fs = boost::filesystem;

namespace TransModel { 

const std::string BALANCED = "balanced";
const double VS_VL_COUNT = std::log10(2) + 2;

//CSVWriter csvwriter("jailStatsTest.csv"); //temp csv writer for test
//CSVWriter csvwriter("infec_incidence.csv"); //temp csv writer for test
//int new_infected_this_cycle=0; //couting total infections, includig externals during one cycle sim

PartnershipEvent::PEventType cod_to_PEvent(CauseOfDeath cod) {
    if (cod == CauseOfDeath::AGE)
        return PartnershipEvent::PEventType::ENDED_AGING_OUT;
    else if (cod == CauseOfDeath::ASM)
        return PartnershipEvent::PEventType::ENDED_DEATH_ASM;
    else if (cod == CauseOfDeath::INFECTION)
        return PartnershipEvent::PEventType::ENDED_DEATH_INFECTION;
    else if (cod == CauseOfDeath::ASM_CD4) 
        return PartnershipEvent::PEventType::ENDED_DEATH_ASM_CD4;
    else {
        throw std::invalid_argument("No PEvent for specified CauseOfDeath");
    }
}

struct PersonToVALForSimulate {

    List operator()(const PersonPtr& v, int idx, double tick) const {

        return List::create(Named("na") = false, Named("vertex_names") = idx, Named("role_main") = v->steady_role(),
                Named("role_casual") = v->casual_role(), Named("inf.status") = v->isInfected(),
                Named("diagnosed") = v->isDiagnosed(), Named("age") = v->age(), Named("sqrt.age") = sqrt(v->age()));
    }
};

shared_ptr<TransmissionRunner> create_transmission_runner() {
    float circ_mult = (float) Parameters::instance()->getDoubleParameter(CIRCUM_MULT);
    float condom_mult = (float) Parameters::instance()->getDoubleParameter(INFECTIVITY_REDUCTION_CONDOM);
    float infective_insertive_mult = (float) Parameters::instance()->getDoubleParameter(INFECTIVE_INSERTIVE_MULT);

    float duration = Parameters::instance()->getFloatParameter(DURATION_OF_INFECTION);
    return make_shared<TransmissionRunner>(circ_mult, condom_mult, infective_insertive_mult, duration);
}

CD4Calculator create_CD4Calculator() {
    // float size_of_timestep, float cd4_recovery_time,
    // float cd4_at_infection_male, float per_day_cd4_recovery
    float size_of_timestep = (float) Parameters::instance()->getDoubleParameter(SIZE_OF_TIMESTEP);
    float cd4_recovery_time = (float) Parameters::instance()->getDoubleParameter(CD4_RECOVERY_TIME);
    float cd4_at_infection = (float) Parameters::instance()->getDoubleParameter(CD4_AT_INFECTION_MALE);
    float per_day_cd4_recovery = (float) Parameters::instance()->getDoubleParameter(PER_DAY_CD4_RECOVERY);

    BValues b_values;
    b_values.b1_ref = Parameters::instance()->getFloatParameter(B1_REF);
    b_values.b2_african = Parameters::instance()->getFloatParameter(B2_AFRICAN);
    b_values.b3_female = Parameters::instance()->getFloatParameter(B3_FEMALE);
    b_values.b4_cd4_ref = Parameters::instance()->getFloatParameter(B4_CD4_REF);
    b_values.b5_african = Parameters::instance()->getFloatParameter(B5_AFRICAN);
    b_values.b6_age_15to29 = Parameters::instance()->getFloatParameter(B6_AGE_15TO29);
    b_values.b6_age_30to39 = Parameters::instance()->getFloatParameter(B6_AGE_30TO39);
    b_values.b6_age_40to49 = Parameters::instance()->getFloatParameter(B6_AGE_40TO49);
    b_values.b6_age_50ormore = Parameters::instance()->getFloatParameter(B6_AGE_50ORMORE);
    return CD4Calculator(size_of_timestep, cd4_recovery_time, cd4_at_infection, per_day_cd4_recovery, b_values);
}

ViralLoadCalculator create_ViralLoadCalculator() {
    SharedViralLoadParameters params;
    params.time_infection_to_peak_load = (float) Parameters::instance()->getDoubleParameter(
            TIME_INFECTION_TO_PEAK_VIRAL_LOAD);
    params.time_infection_to_set_point = (float) Parameters::instance()->getDoubleParameter(
            TIME_INFECTION_TO_VIRAL_SET_POINT);
    params.time_infection_to_late_stage = (float) Parameters::instance()->getDoubleParameter(
            TIME_INFECTION_TO_LATE_STAGE);
    params.time_to_full_supp = (float) Parameters::instance()->getDoubleParameter(TIME_TO_FULL_SUPP);
    params.peak_viral_load = (float) Parameters::instance()->getDoubleParameter(PEAK_VIRAL_LOAD);
    params.set_point_viral_load = (float) Parameters::instance()->getDoubleParameter(SET_POINT_VIRAL_LOAD);
    params.late_stage_viral_load = (float) Parameters::instance()->getDoubleParameter(LATE_STAGE_VIRAL_LOAD);
    params.undetectable_viral_load = (float) Parameters::instance()->getDoubleParameter(UNDETECTABLE_VL);

    return ViralLoadCalculator(params);
}

ViralLoadSlopeCalculator create_ViralLoadSlopeCalculator() {
    float time_to_full_supp = Parameters::instance()->getFloatParameter(TIME_TO_FULL_SUPP);
    float undetectable_vl = Parameters::instance()->getFloatParameter(UNDETECTABLE_VL);
    return ViralLoadSlopeCalculator(undetectable_vl, time_to_full_supp);
}

void init_stage_map(map<float, shared_ptr<Stage>> &stage_map) {
    float acute_max = (float) Parameters::instance()->getDoubleParameter(ACUTE_LENGTH_MAX);
    float chronic_max = (float) Parameters::instance()->getDoubleParameter(CHRONIC_LENGTH_MAX);
    //float late_max = (float) Parameters::instance()->getDoubleParameter(LATE_LENGTH_MAX);
    float acute_mult = (float) Parameters::instance()->getDoubleParameter(ACUTE_MULT);
    float late_mult = (float) Parameters::instance()->getDoubleParameter(LATE_MULT);
    float baseline_infectivity = (float) Parameters::instance()->getDoubleParameter(MIN_CHRONIC_INFECTIVITY_UNADJ);
    float viral_load_incr = (float) Parameters::instance()->getDoubleParameter(VIRAL_LOAD_LOG_INCREMENT);

    stage_map.emplace(acute_max,
            make_shared<AcuteStage>(baseline_infectivity, acute_mult, Range<float>(1, acute_max), viral_load_incr));
    stage_map.emplace(chronic_max,
            make_shared<ChronicStage>(baseline_infectivity, Range<float>(acute_max, chronic_max), viral_load_incr));
    // make late_max essentially open ended as infected persons
    // on ART can be in late stage forever where stage is not necessarily
    // the medical stage, but the Stage class for our purposes.
    float late_max = std::numeric_limits<float>::max();
    stage_map.emplace(late_max,
            make_shared<LateStage>(baseline_infectivity, late_mult, Range<float>(chronic_max, late_max),
                    viral_load_incr));
}

void init_generators() {
    float circum_rate = Parameters::instance()->getDoubleParameter(CIRCUM_RATE);
    BinomialGen rate(repast::Random::instance()->engine(), boost::random::binomial_distribution<>(1, circum_rate));
    Random::instance()->putGenerator(CIRCUM_STATUS_BINOMIAL, new DefaultNumberGenerator<BinomialGen>(rate));
}

std::string get_stats_filename(const std::string& key) {
    if (Parameters::instance()->contains(key)) {
        return Parameters::instance()->getStringParameter(key);
    } else {
        return "";
    }
}

void init_stats() {
    // 19-30,30-100
    StatsBuilder builder(output_directory(Parameters::instance()));
    builder.countsWriter(get_stats_filename(COUNTS_PER_TIMESTEP_OUTPUT_FILE));
    builder.partnershipEventWriter(get_stats_filename(PARTNERSHIP_EVENTS_FILE));
    builder.infectionEventWriter(get_stats_filename(INFECTION_EVENTS_FILE));
    builder.biomarkerWriter(get_stats_filename(BIOMARKER_FILE));
    builder.deathEventWriter(get_stats_filename(DEATH_EVENT_FILE));
    builder.personDataRecorder(get_stats_filename(PERSON_DATA_FILE));
    builder.testingEventWriter(get_stats_filename(TESTING_EVENT_FILE));
    builder.artEventWriter(get_stats_filename(ART_EVENT_FILE));
    builder.prepEventWriter(get_stats_filename(PREP_EVENT_FILE));

    int min_age = Parameters::instance()->getIntParameter(MIN_AGE);
    int max_age = Parameters::instance()->getIntParameter(MAX_AGE);

    builder.createStatsSingleton(min_age, max_age);

    //temp csv writer for test: 
    //std::vector<std::string> header = {"tick", "newJailed", "nextDayRelease", "jailedPop", "generalPop", "totalPop", 
                                      // "popChange", "entries", "ageDeaths", "infectDeaths", "asmDeaths", "totalDeaths"};
    //std::vector<std::string> header = {"baseline"};
    std::vector<std::string> header = {"inc_rate", "incar_rate", "jail_pop",  
    "tot_new_inf_jail", "tot_new_infected", "uninfected_c", "tot_pop",
    "internal_infected", "external_infected", "infected_at_entry", "uninfected"};

    //csvwriter.addHeader(header);
}

void add_log(const std::string& fname, const std::string& header, 
    const std::string& out_dir, int LOG_TAG) {
        
     if (fname.size() > 0) {
        std::string full_fname = out_dir + "/" + fname;
        Logger::instance()->addLog(LOG_TAG, full_fname);
        std::shared_ptr<Log> log = Logger::instance()->getLog(LOG_TAG);
        // add header
        (*log) << header << "\n";
    }
}

void init_logs() {
    std::string out_dir = Parameters::instance()->getStringParameter(OUTPUT_DIR);

    std::string fname = get_stats_filename(BASE_LOG_FILE);
    add_log(fname, "tick,candidate_count,selected,off_prep_adjustment,probability", out_dir, BASE_LOG);

    const std::string prep_scheme = Parameters::instance()->getStringParameter(PREP_SCHEME);

    if (prep_scheme == "serodiscordant") {
        fname = get_stats_filename(SERO_LOG_FILE);
        add_log(fname, "tick,candidate_count,selected,off_prep_adjustment,probability", out_dir, SERO_LOG);
    } else if (prep_scheme == "eigen" || prep_scheme == "degree") {
        fname = get_stats_filename(NET_LOG_FILE);
        add_log(fname, "tick,candidate_count,top_n_count,selected,off_prep_adjustment,probability", out_dir, NET_LOG);
    } else if (prep_scheme == "default") {
        fname = get_stats_filename(RANDOM_SELECTION_LOG_FILE);
        add_log(fname,  "tick,candidate_count,selected,off_prep_adjustment,probability", out_dir, RANDOM_SELECTION_LOG);
    }
}

void init_network_save(Jail* jail, Network<Person>* net, std::shared_ptr<RInside> R) {
    string save_prop = Parameters::instance()->getStringParameter(NET_SAVE_AT);
    vector<string> ats;
    boost::split(ats, save_prop, boost::is_any_of(","));
    if (ats.size() > 0) {
        ScheduleRunner& runner = RepastProcess::instance()->getScheduleRunner();
        for (auto& at : ats) {
            boost::trim(at);
            Serializer* serializer = new Serializer(net, jail, R);

            if (at == "end") {
                //std::cout << "scheduling at end" << std::endl;
                runner.scheduleEndEvent(Schedule::FunctorPtr(serializer));
            } else {
                double tick = stod(at);
                //std::cout << "scheduling at " << tick << std::endl;
                runner.scheduleEvent(tick + 0.1,
                        Schedule::FunctorPtr(serializer));
            }
        }
    }
}

void init_biomarker_logging(size_t pop_size, std::set<int>& ids_to_log) {
    if (Parameters::instance()->contains(BIOMARKER_LOG_COUNT) && Parameters::instance()->contains(BIOMARKER_FILE)) {
        string bio_param = Parameters::instance()->getStringParameter(BIOMARKER_LOG_COUNT);
        if (bio_param.find("random:") != string::npos) {
            int number_to_log = stoi(bio_param.substr(7));
       
            IntUniformGenerator gen = Random::instance()->createUniIntGenerator(0, pop_size - 1);
            for (int i = 0; i < number_to_log; ++i) {
                int id = (int) gen.next();
                while (ids_to_log.find(id) != ids_to_log.end()) {
                    id = (int) gen.next();
                }
                ids_to_log.emplace(id);
            }
        } else {
            vector<string> ids;
            boost::split(ids, bio_param, boost::is_any_of(","));
            for (auto& sid : ids) {
                ids_to_log.emplace(stoi(sid));
            }
        }
    }
}

void init_trans_params(TransmissionParameters& params) {
    double size_of_time_step = Parameters::instance()->getDoubleParameter(SIZE_OF_TIMESTEP);
    params.prop_steady_sex_acts = Parameters::instance()->getDoubleParameter(PROP_STEADY_SEX_ACTS) * size_of_time_step;
    params.prop_casual_sex_acts = Parameters::instance()->getDoubleParameter(PROP_CASUAL_SEX_ACTS) * size_of_time_step;
}

NetworkType find_net_type(std::string type) {
    if (type == "main") return NetworkType::MAIN;
    if (type == "casual") return NetworkType::CASUAL;
    if (type == "all") return NetworkType::ALL;
    throw std::invalid_argument("Unknown partnership type for PrEP serodiscordant intervention: '" + type + "'");
}

void init_sero_intervention(std::vector<std::shared_ptr<IPrepIntervention>>& vec, float age_threshold, float max_age) {
    PrepUptakeData lt_base_data, gte_base_data;
    lt_base_data.years_to_increment = 0;
    gte_base_data.years_to_increment = 0;
    lt_base_data.increment = 0;
    gte_base_data.increment =0;
    
    // used in base prob calc
    lt_base_data.use = Parameters::instance()->getDoubleParameter(SERO_BASE_PREP_USE_PROP_LT);
    gte_base_data.use = Parameters::instance()->getDoubleParameter(SERO_BASE_PREP_USE_PROP_GTE);

    // used in cessation generators
    lt_base_data.cessation_stop = Parameters::instance()->getDoubleParameter(SERO_BASE_PREP_DAILY_STOP_PROB_LT);
    gte_base_data.cessation_stop = Parameters::instance()->getDoubleParameter(SERO_BASE_PREP_DAILY_STOP_PROB_GTE);

    // used to calculate base probabilities
    lt_base_data.stop = Parameters::instance()->getDoubleParameter(SERO_BASE_PREP_DAILY_STOP_PROB_LT);
    gte_base_data.stop = Parameters::instance()->getDoubleParameter(SERO_BASE_PREP_DAILY_STOP_PROB_GTE);

    // add the base prep intervention
    std::shared_ptr<LTPrepAgeFilter> lt_base = std::make_shared<LTPrepAgeFilter>(age_threshold);
    std::shared_ptr<GTEPrepAgeFilter> gte_base = std::make_shared<GTEPrepAgeFilter>(age_threshold, max_age);
    vec.push_back(std::make_shared<BasePrepIntervention>(lt_base_data, lt_base));
    vec.push_back(std::make_shared<BasePrepIntervention>(gte_base_data, gte_base));

    PrepUptakeData lt_sd, gte_sd;
    lt_sd.use = lt_base_data.use;
    gte_sd.use = gte_base_data.use;

    lt_sd.stop = Parameters::instance()->getDoubleParameter(SERO_INTRV_PREP_DAILY_STOP_PROB_LT);
    gte_sd.stop = Parameters::instance()->getDoubleParameter(SERO_INTRV_PREP_DAILY_STOP_PROB_GTE);
    lt_sd.cessation_stop = Parameters::instance()->getDoubleParameter(SERO_INTRV_PREP_DAILY_STOP_PROB_LT);
    gte_sd.cessation_stop =Parameters::instance()->getDoubleParameter(SERO_INTRV_PREP_DAILY_STOP_PROB_GTE);

    lt_sd.increment = Parameters::instance()->getDoubleParameter(SERO_PREP_YEARLY_INCREMENT_LT);
    gte_sd.increment = Parameters::instance()->getDoubleParameter(SERO_PREP_YEARLY_INCREMENT_GTE);
    lt_sd.years_to_increment = Parameters::instance()->getDoubleParameter(SERO_PREP_YEARS_TO_INCREMENT);
    gte_sd.years_to_increment = Parameters::instance()->getDoubleParameter(SERO_PREP_YEARS_TO_INCREMENT);

    NetworkType net_type = find_net_type(Parameters::instance()->getStringParameter(SERO_NET_TYPE));

    // these are immutable so we could reuse them, but they may not remain so in the future
    std::shared_ptr<LTPrepAgeFilter> lt = std::make_shared<LTPrepAgeFilter>(age_threshold);
    std::shared_ptr<GTEPrepAgeFilter> gte = std::make_shared<GTEPrepAgeFilter>(age_threshold, max_age);
   
    vec.push_back(std::make_shared<SerodiscordantPrepIntervention>(lt_sd, lt, net_type));
    vec.push_back(std::make_shared<SerodiscordantPrepIntervention>(gte_sd, gte, net_type));

    std::cout << "SD Net Type: " << Parameters::instance()->getStringParameter(SERO_NET_TYPE) << ", " 
        << static_cast<int>(net_type) << std::endl;
    std::cout << "SD Base lt: " << lt_base_data << "\n";
    std::cout << "SD Base gte: " << gte_base_data << "\n";
    std::cout << "SD Intrv lt: " << lt_sd << "\n";
    std::cout << "SD Intrv gte: " << gte_sd << "\n";


    // // used in PUExtras to calculate base unboosted probability
    // data.daily_stop_prob_sd_lt = Parameters::instance()->getDoubleParameter(SERO_INTRV_PREP_DAILY_STOP_PROB_LT);
    // data.daily_stop_prob_sd_gte = Parameters::instance()->getDoubleParameter(SERO_INTRV_PREP_DAILY_STOP_PROB_GTE);

    // data.increment_sd_lt = Parameters::instance()->getDoubleParameter(SERO_PREP_YEARLY_INCREMENT_LT);
    // data.increment_sd_gte = Parameters::instance()->getDoubleParameter(SERO_PREP_YEARLY_INCREMENT_GTE);
    // data.years_to_increase = Parameters::instance()->getDoubleParameter(SERO_PREP_YEARS_TO_INCREMENT);

    // string net_type = Parameters::instance()->getStringParameter(SERO_NET_TYPE);

    // std::cout << "prep.update=serodiscordant" << "\n" << data << net_type << std::endl;    
}

void init_eigen_intervention(std::vector<std::shared_ptr<IPrepIntervention>>& vec, float age_threshold, float max_age) {
    PrepUptakeData lt_base_data, gte_base_data;
    lt_base_data.years_to_increment = 0;
    gte_base_data.years_to_increment = 0;
    lt_base_data.increment = 0;
    gte_base_data.increment =0;
    
    // used in base prob calc
    lt_base_data.use = Parameters::instance()->getDoubleParameter(EIGEN_BASE_PREP_USE_PROP_LT);
    gte_base_data.use = Parameters::instance()->getDoubleParameter(EIGEN_BASE_PREP_USE_PROP_GTE);

    // used in cessation generators
    lt_base_data.cessation_stop = Parameters::instance()->getDoubleParameter(EIGEN_BASE_PREP_DAILY_STOP_PROB_LT);
    gte_base_data.cessation_stop = Parameters::instance()->getDoubleParameter(EIGEN_BASE_PREP_DAILY_STOP_PROB_GTE);

    // used to calculate base probabilities
    lt_base_data.stop = Parameters::instance()->getDoubleParameter(EIGEN_BASE_PREP_DAILY_STOP_PROB_LT);
    gte_base_data.stop = Parameters::instance()->getDoubleParameter(EIGEN_BASE_PREP_DAILY_STOP_PROB_GTE);

    // these are immutable so we could reuse them, but they may not remain so in the future
    std::shared_ptr<LTPrepAgeFilter> lt_base = std::make_shared<LTPrepAgeFilter>(age_threshold);
    std::shared_ptr<GTEPrepAgeFilter> gte_base = std::make_shared<GTEPrepAgeFilter>(age_threshold, max_age);

    // add the base prep intervention
    vec.push_back(std::make_shared<BasePrepIntervention>(lt_base_data, lt_base));
    vec.push_back(std::make_shared<BasePrepIntervention>(gte_base_data, gte_base));

    PrepUptakeData lt_data, gte_data;
    lt_data.use = lt_base_data.use;
    gte_data.use = gte_base_data.use;

    lt_data.stop = Parameters::instance()->getDoubleParameter(EIGEN_INTRV_PREP_DAILY_STOP_PROB_LT);
    gte_data.stop = Parameters::instance()->getDoubleParameter(EIGEN_INTRV_PREP_DAILY_STOP_PROB_GTE);
    lt_data.cessation_stop = Parameters::instance()->getDoubleParameter(EIGEN_INTRV_PREP_DAILY_STOP_PROB_LT);
    gte_data.cessation_stop = Parameters::instance()->getDoubleParameter(EIGEN_INTRV_PREP_DAILY_STOP_PROB_GTE);

    lt_data.increment = Parameters::instance()->getDoubleParameter(EIGEN_PREP_YEARLY_INCREMENT_LT);
    gte_data.increment = Parameters::instance()->getDoubleParameter(EIGEN_PREP_YEARLY_INCREMENT_GTE);
    lt_data.years_to_increment = Parameters::instance()->getDoubleParameter(EIGEN_PREP_YEARS_TO_INCREMENT);
    gte_data.years_to_increment = Parameters::instance()->getDoubleParameter(EIGEN_PREP_YEARS_TO_INCREMENT);

    std::shared_ptr<CompositeNetStatPrepIntervention> intervention = std::make_shared<CompositeNetStatPrepIntervention>(&eigen_ranker);
    float top_n = (float)Parameters::instance()->getDoubleParameter(EIGEN_TOPN);

    // these are immutable so we could reuse them, but they may not remain so in the future
    std::shared_ptr<LTPrepAgeFilter> lt = std::make_shared<LTPrepAgeFilter>(age_threshold);
    std::shared_ptr<GTEPrepAgeFilter> gte = std::make_shared<GTEPrepAgeFilter>(age_threshold, max_age);

    intervention->addNetIntervention(std::make_shared<NetStatPrepIntervention>(lt_data, lt, top_n));
    intervention->addNetIntervention(std::make_shared<NetStatPrepIntervention>(gte_data, gte, top_n));
    vec.push_back(intervention);

    std::cout << "EIGEN top_n: " << top_n << "\n";
    std::cout << "EIGEN Base lt: " << lt_base_data << "\n";
    std::cout << "EIGEN Base gte: " << gte_base_data << "\n";
    std::cout << "EIGEN Intrv lt: " << lt_data << "\n";
    std::cout << "EIGEN Intrv gte: " << gte_data << "\n";
    
    // PrepUseData data;

    // // used in PUBBase as k in prob calc
    // data.base_use_lt = Parameters::instance()->getDoubleParameter(EIGEN_BASE_PREP_USE_PROP_LT);
    // data.base_use_gte = Parameters::instance()->getDoubleParameter(EIGEN_BASE_PREP_USE_PROP_GTE);

    // // used in PrepUptakeManager in cessation generators
    // data.daily_stop_prob_lt = Parameters::instance()->getDoubleParameter(EIGEN_BASE_PREP_DAILY_STOP_PROB_LT);
    // data.daily_stop_prob_gte = Parameters::instance()->getDoubleParameter(EIGEN_BASE_PREP_DAILY_STOP_PROB_GTE);

    // // used in PUExtras to calculate base unboosted probability
    // data.daily_stop_prob_net_lt = Parameters::instance()->getDoubleParameter(EIGEN_INTRV_PREP_DAILY_STOP_PROB_LT);
    // data.daily_stop_prob_net_gte = Parameters::instance()->getDoubleParameter(EIGEN_INTRV_PREP_DAILY_STOP_PROB_LT);

    // // used in PUBase to calculated base probabilities
    // data.daily_p_prob_lt = Parameters::instance()->getDoubleParameter(EIGEN_BASE_PREP_DAILY_STOP_PROB_LT);
    // data.daily_p_prob_gte = Parameters::instance()->getDoubleParameter(EIGEN_BASE_PREP_DAILY_STOP_PROB_GTE);

    // data.increment_net_lt = Parameters::instance()->getDoubleParameter(EIGEN_PREP_YEARLY_INCREMENT_LT);
    // data.increment_net_gte = Parameters::instance()->getDoubleParameter(EIGEN_PREP_YEARLY_INCREMENT_GTE);
    // data.years_to_increase = Parameters::instance()->getDoubleParameter(EIGEN_PREP_YEARS_TO_INCREMENT);

    // float topn = (float)Parameters::instance()->getDoubleParameter(EIGEN_TOPN);

    // std::cout << data << topn << std::endl;
    // return std::make_shared<EigenPUManager>(data, age_threshold, topn);
}

// std::shared_ptr<DegreePUManager> create_degree_intervention(float age_threshold) {
//     PrepUseData data;

//     // used in PUBBase as k in prob calc
//     data.base_use_lt = Parameters::instance()->getDoubleParameter(DEGREE_BASE_PREP_USE_PROP_LT);
//     data.base_use_gte = Parameters::instance()->getDoubleParameter(DEGREE_BASE_PREP_USE_PROP_GTE);

//     // used in PrepUptakeManager in cessation generators
//     data.daily_stop_prob_lt = Parameters::instance()->getDoubleParameter(DEGREE_BASE_PREP_DAILY_STOP_PROB_LT);
//     data.daily_stop_prob_gte = Parameters::instance()->getDoubleParameter(DEGREE_BASE_PREP_DAILY_STOP_PROB_GTE);

//     // used in PUExtras to calculate base unboosted probability
//     data.daily_stop_prob_net_lt = Parameters::instance()->getDoubleParameter(DEGREE_INTRV_PREP_DAILY_STOP_PROB_LT);
//     data.daily_stop_prob_net_gte = Parameters::instance()->getDoubleParameter(DEGREE_INTRV_PREP_DAILY_STOP_PROB_LT);

//     // used in PUBase to calculated base probabilities
//     data.daily_p_prob_lt = Parameters::instance()->getDoubleParameter(DEGREE_BASE_PREP_DAILY_STOP_PROB_LT);
//     data.daily_p_prob_gte = Parameters::instance()->getDoubleParameter(DEGREE_BASE_PREP_DAILY_STOP_PROB_GTE);

//     data.increment_net_lt = Parameters::instance()->getDoubleParameter(DEGREE_PREP_YEARLY_INCREMENT_LT);
//     data.increment_net_gte = Parameters::instance()->getDoubleParameter(DEGREE_PREP_YEARLY_INCREMENT_GTE);
//     data.years_to_increase = Parameters::instance()->getDoubleParameter(DEGREE_PREP_YEARS_TO_INCREMENT);

//     float topn = (float)Parameters::instance()->getDoubleParameter(DEGREE_TOPN);

//     std::cout << data << topn << std::endl;
//     return std::make_shared<DegreePUManager>(data, age_threshold, topn);
// }

void init_degree_intervention(std::vector<std::shared_ptr<IPrepIntervention>>& vec, float age_threshold, float max_age) {
    PrepUptakeData lt_base_data, gte_base_data;
    lt_base_data.years_to_increment = 0;
    gte_base_data.years_to_increment = 0;
    lt_base_data.increment = 0;
    gte_base_data.increment =0;
    
    // used in base prob calc
    lt_base_data.use = Parameters::instance()->getDoubleParameter(DEGREE_BASE_PREP_USE_PROP_LT);
    gte_base_data.use = Parameters::instance()->getDoubleParameter(DEGREE_BASE_PREP_USE_PROP_GTE);

    // used in cessation generators
    lt_base_data.cessation_stop = Parameters::instance()->getDoubleParameter(DEGREE_BASE_PREP_DAILY_STOP_PROB_LT);
    gte_base_data.cessation_stop = Parameters::instance()->getDoubleParameter(DEGREE_BASE_PREP_DAILY_STOP_PROB_GTE);

    // used to calculate base probabilities
    lt_base_data.stop = Parameters::instance()->getDoubleParameter(DEGREE_BASE_PREP_DAILY_STOP_PROB_LT);
    gte_base_data.stop = Parameters::instance()->getDoubleParameter(DEGREE_BASE_PREP_DAILY_STOP_PROB_GTE);

    // these are immutable so we could reuse them, but they may not remain so in the future
    std::shared_ptr<LTPrepAgeFilter> lt_base = std::make_shared<LTPrepAgeFilter>(age_threshold);
    std::shared_ptr<GTEPrepAgeFilter> gte_base = std::make_shared<GTEPrepAgeFilter>(age_threshold, max_age);

    // add the base prep intervention
    vec.push_back(std::make_shared<BasePrepIntervention>(lt_base_data, lt_base));
    vec.push_back(std::make_shared<BasePrepIntervention>(gte_base_data, gte_base));

    PrepUptakeData lt_data, gte_data;
    lt_data.use = lt_base_data.use;
    gte_data.use = gte_base_data.use;

    lt_data.stop = Parameters::instance()->getDoubleParameter(DEGREE_INTRV_PREP_DAILY_STOP_PROB_LT);
    gte_data.stop = Parameters::instance()->getDoubleParameter(DEGREE_INTRV_PREP_DAILY_STOP_PROB_GTE);
    lt_data.cessation_stop = Parameters::instance()->getDoubleParameter(DEGREE_INTRV_PREP_DAILY_STOP_PROB_LT);
    gte_data.cessation_stop = Parameters::instance()->getDoubleParameter(DEGREE_INTRV_PREP_DAILY_STOP_PROB_GTE);

    lt_data.increment = Parameters::instance()->getDoubleParameter(DEGREE_PREP_YEARLY_INCREMENT_LT);
    gte_data.increment = Parameters::instance()->getDoubleParameter(DEGREE_PREP_YEARLY_INCREMENT_GTE);
    lt_data.years_to_increment = Parameters::instance()->getDoubleParameter(DEGREE_PREP_YEARS_TO_INCREMENT);
    gte_data.years_to_increment = Parameters::instance()->getDoubleParameter(DEGREE_PREP_YEARS_TO_INCREMENT);

    std::shared_ptr<CompositeNetStatPrepIntervention> intervention = std::make_shared<CompositeNetStatPrepIntervention>(&degree_ranker);
    float top_n = (float)Parameters::instance()->getDoubleParameter(DEGREE_TOPN);

    // these are immutable so we could reuse them, but they may not remain so in the future
    std::shared_ptr<LTPrepAgeFilter> lt = std::make_shared<LTPrepAgeFilter>(age_threshold);
    std::shared_ptr<GTEPrepAgeFilter> gte = std::make_shared<GTEPrepAgeFilter>(age_threshold, max_age);

    intervention->addNetIntervention(std::make_shared<NetStatPrepIntervention>(lt_data, lt, top_n));
    intervention->addNetIntervention(std::make_shared<NetStatPrepIntervention>(gte_data, gte, top_n));
    vec.push_back(intervention);

    std::cout << "DEGREE top_n: " << top_n << "\n";
    std::cout << "DEGREE Base lt: " << lt_base_data << "\n";
    std::cout << "DEGREE Base gte: " << gte_base_data << "\n";
    std::cout << "DEGREE Intrv lt: " << lt_data << "\n";
    std::cout << "DEGREE Intrv gte: " << gte_data << "\n";
}

void init_random_intervention(std::vector<std::shared_ptr<IPrepIntervention>>& vec, float age_threshold, float max_age) {
    PrepUptakeData lt_base_data, gte_base_data;
    lt_base_data.years_to_increment = 0;
    gte_base_data.years_to_increment = 0;
    lt_base_data.increment = 0;
    gte_base_data.increment = 0;
    
    // used in base prob calc
    lt_base_data.use = Parameters::instance()->getDoubleParameter(RANDOM_BASE_PREP_USE_PROP_LT);
    gte_base_data.use = Parameters::instance()->getDoubleParameter(RANDOM_BASE_PREP_USE_PROP_GTE);

    // used in cessation generators
    lt_base_data.cessation_stop = Parameters::instance()->getDoubleParameter(RANDOM_BASE_PREP_DAILY_STOP_PROB_LT);
    gte_base_data.cessation_stop = Parameters::instance()->getDoubleParameter(RANDOM_BASE_PREP_DAILY_STOP_PROB_GTE);

    // used to calculate base probabilities
    lt_base_data.stop = Parameters::instance()->getDoubleParameter(RANDOM_BASE_PREP_DAILY_STOP_PROB_LT);
    gte_base_data.stop = Parameters::instance()->getDoubleParameter(RANDOM_BASE_PREP_DAILY_STOP_PROB_GTE);

    // these are immutable so we could reuse them, but they may not remain so in the future
    std::shared_ptr<LTPrepAgeFilter> lt_base = std::make_shared<LTPrepAgeFilter>(age_threshold);
    std::shared_ptr<GTEPrepAgeFilter> gte_base = std::make_shared<GTEPrepAgeFilter>(age_threshold, max_age);

    // add the base prep intervention
    vec.push_back(std::make_shared<BasePrepIntervention>(lt_base_data, lt_base));
    vec.push_back(std::make_shared<BasePrepIntervention>(gte_base_data, gte_base));

    PrepUptakeData lt_data, gte_data;
    lt_data.use = lt_base_data.use;
    gte_data.use = gte_base_data.use;

    lt_data.stop = Parameters::instance()->getDoubleParameter(RANDOM_INTRV_PREP_DAILY_STOP_PROB_LT);
    gte_data.stop = Parameters::instance()->getDoubleParameter(RANDOM_INTRV_PREP_DAILY_STOP_PROB_GTE);
    lt_data.cessation_stop = Parameters::instance()->getDoubleParameter(RANDOM_INTRV_PREP_DAILY_STOP_PROB_LT);
    gte_data.cessation_stop = Parameters::instance()->getDoubleParameter(RANDOM_INTRV_PREP_DAILY_STOP_PROB_GTE);

    lt_data.increment = Parameters::instance()->getDoubleParameter(RANDOM_PREP_YEARLY_INCREMENT_LT);
    gte_data.increment = Parameters::instance()->getDoubleParameter(RANDOM_PREP_YEARLY_INCREMENT_GTE);
    lt_data.years_to_increment = Parameters::instance()->getDoubleParameter(RANDOM_PREP_YEARS_TO_INCREMENT);
    gte_data.years_to_increment = Parameters::instance()->getDoubleParameter(RANDOM_PREP_YEARS_TO_INCREMENT);

    // these are immutable so we could reuse them, but they may not remain so in the future
    std::shared_ptr<LTPrepAgeFilter> lt = std::make_shared<LTPrepAgeFilter>(age_threshold);
    std::shared_ptr<GTEPrepAgeFilter> gte = std::make_shared<GTEPrepAgeFilter>(age_threshold, max_age);

    vec.push_back(std::make_shared<RandomSelectionPrepIntervention>(lt_data, lt));
    vec.push_back(std::make_shared<RandomSelectionPrepIntervention>(gte_data, gte));

    std::cout << "Random Intervention lt: " << lt_data << "\n";
    std::cout << "Random Intervention gte: " << gte_data << "\n";
    
}

void init_default_intervention(std::vector<std::shared_ptr<IPrepIntervention>>& vec, float age_threshold, float max_age) {
    // Add the base
    PrepUptakeData lt_base_data, gte_base_data;
    lt_base_data.years_to_increment = 0;
    gte_base_data.years_to_increment = 0;
    lt_base_data.increment = 0;
    gte_base_data.increment =0;    

    lt_base_data.use = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_USE_PROP_LT);
    gte_base_data.use =  Parameters::instance()->getDoubleParameter(DEFAULT_PREP_USE_PROP_GTE);
    lt_base_data.cessation_stop = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_DAILY_STOP_PROB_LT);
    gte_base_data.cessation_stop = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_DAILY_STOP_PROB_GTE);

    bool balanced = Parameters::instance()->getStringParameter(DEFAULT_PREP_BALANCED_UNBALANCED) == BALANCED;
    if (balanced) {
        lt_base_data.stop = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_DAILY_STOP_PROB_LT);
        gte_base_data.stop = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_DAILY_STOP_PROB_GTE);
    } else {
        lt_base_data.stop = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_UNBALANCED_STARTING_PROB_LT);
        gte_base_data.stop = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_UNBALANCED_STARTING_PROB_GTE);
    }

    // these are immutable so we could reuse them, but they may not remain so in the future
    std::shared_ptr<LTPrepAgeFilter> lt_base = std::make_shared<LTPrepAgeFilter>(age_threshold);
    std::shared_ptr<GTEPrepAgeFilter> gte_base = std::make_shared<GTEPrepAgeFilter>(age_threshold, max_age);

    vec.push_back(std::make_shared<BasePrepIntervention>(lt_base_data, lt_base));
    vec.push_back(std::make_shared<BasePrepIntervention>(gte_base_data, gte_base));

    // Add the intervention
    PrepUptakeData lt_data, gte_data;
    lt_data.use = lt_base_data.use;
    gte_data.use = gte_base_data.use;

    lt_data.stop = lt_base_data.stop;
    gte_data.stop = gte_base_data.stop;
    lt_data.cessation_stop = lt_base_data.cessation_stop;
    gte_data.cessation_stop = gte_base_data.cessation_stop;

    lt_data.increment = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_YEARLY_INCREMENT_LT);
    gte_data.increment = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_YEARLY_INCREMENT_GTE);
    lt_data.years_to_increment = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_YEARS_TO_INCREMENT);
    gte_data.years_to_increment = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_YEARS_TO_INCREMENT);

    // these are immutable so we could reuse them, but they may not remain so in the future
    std::shared_ptr<LTPrepAgeFilter> lt = std::make_shared<LTPrepAgeFilter>(age_threshold);
    std::shared_ptr<GTEPrepAgeFilter> gte = std::make_shared<GTEPrepAgeFilter>(age_threshold, max_age);

    vec.push_back(std::make_shared<RandomSelectionPrepIntervention>(lt_data, lt));
    vec.push_back(std::make_shared<RandomSelectionPrepIntervention>(gte_data, gte));

    std::cout << "Random Default Selection lt: " << lt_data << "\n";
    std::cout << "Random Default Selection gte: " << gte_data << "\n";
    
    // PrepUseData data;
    // data.base_use_lt = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_USE_PROP_LT);
    // data.base_use_gte = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_USE_PROP_GTE);
    // data.daily_stop_prob_lt = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_DAILY_STOP_PROB_LT);
    // data.daily_stop_prob_gte = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_DAILY_STOP_PROB_GTE);
    
    // data.increment_lt = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_YEARLY_INCREMENT_LT);
    // data.increment_gte = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_YEARLY_INCREMENT_GTE);
    // data.years_to_increase = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_YEARS_TO_INCREMENT);

    // bool balanced = Parameters::instance()->getStringParameter(DEFAULT_PREP_BALANCED_UNBALANCED) == BALANCED;
    // if (balanced) {
    //     data.daily_p_prob_lt = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_DAILY_STOP_PROB_LT);
    //     data.daily_p_prob_gte = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_DAILY_STOP_PROB_GTE);
    // } else {
    //     data.daily_p_prob_lt = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_UNBALANCED_STARTING_PROB_LT);
    //     data.daily_p_prob_gte = Parameters::instance()->getDoubleParameter(DEFAULT_PREP_UNBALANCED_STARTING_PROB_GTE);
    // }

    // std::cout << data << std::endl;

    // return std::make_shared<IncrementingPrepUptakeManager>(data, age_threshold);
}

// std::shared_ptr<ProportionalPrepUptakeManager> create_yor_intervention(float age_threshold) {
//     PrepUseData data;

//     data.base_use_yor = Parameters::instance()->getDoubleParameter(YOR_PREP_USE_PROP);
//     // the cessation rnd generator uses lte and gte but that's not applicable for yor, so we
//     // just make them the same.
//     data.daily_stop_prob_lt =  Parameters::instance()->getDoubleParameter(YOR_PREP_DAILY_STOP_PROB);
//     data.daily_stop_prob_gte =  Parameters::instance()->getDoubleParameter(YOR_PREP_DAILY_STOP_PROB);
//     data.alpha = Parameters::instance()->getDoubleParameter(YOR_PREP_ALPHA);
//     data.increment_yor = Parameters::instance()->getDoubleParameter(YOR_PREP_YEARLY_INCREMENT);
//     data.years_to_increase = Parameters::instance()->getDoubleParameter(YOR_PREP_YEARS_TO_INCREMENT);
//     data.yor_old_extra = Parameters::instance()->getDoubleParameter(YOR_ADDITIONAL_PREP_GTE);
//     data.yor_young_extra = Parameters::instance()->getDoubleParameter(YOR_ADDITIONAL_PREP_LT);

//     std::cout << data << std::endl;

//     return std::make_shared<ProportionalPrepUptakeManager>(data, age_threshold);
// }

void initialize_prep_interventions(PrepInterventionManager& prep_manager) {

    float age_threshold = Parameters::instance()->getFloatParameter(INPUT_AGE_THRESHOLD);
    float max_age =  Parameters::instance()->getFloatParameter(MAX_AGE);
    
    string prep_scheme = Parameters::instance()->getStringParameter(PREP_SCHEME);

    std::vector<std::shared_ptr<IPrepIntervention>> interventions;
    if (prep_scheme == "default") {
        init_default_intervention(interventions, age_threshold, max_age);
    } else if (prep_scheme == "serodiscordant") {
        init_sero_intervention(interventions, age_threshold, max_age);
    } else if (prep_scheme == "eigen") {
        init_eigen_intervention(interventions, age_threshold, max_age);
    } else if (prep_scheme == "degree") {
        init_degree_intervention(interventions, age_threshold, max_age);
    } else {
        throw invalid_argument("Invalid PrEP Uptake scheme: '" + prep_scheme + "'");
    }

    for (auto& intervention : interventions) {
        prep_manager.addIntervention(intervention);
    }

    double intervention_at  = Parameters::instance()->getDoubleParameter(PREP_INTERVENTION_AT);
    if (intervention_at  > 0) {
        interventions.clear();
        string intervention_scheme = Parameters::instance()->getStringParameter(PREP_INTERVENTION_SCHEME);
        if (intervention_scheme == "random") {
            init_random_intervention(interventions, age_threshold, max_age);
        } else if (intervention_scheme == "serodiscordant") {
            init_sero_intervention(interventions, age_threshold, max_age);
        } else if (intervention_scheme == "eigen") {
            init_eigen_intervention(interventions, age_threshold, max_age);
        } else if (intervention_scheme == "degree") {
            init_degree_intervention(interventions, age_threshold, max_age);
        } else {
            throw invalid_argument("Invalid PrEP intervention scheme: '" + prep_scheme + "'");
        }

        std::cout << "Scheduling " << intervention_scheme << " intervention for day " << intervention_at << std::endl;
        boost::shared_ptr<ScheduleIntervention> scheduled_intervention = 
            boost::make_shared<ScheduleIntervention>(&prep_manager);
        for (auto& intervention : interventions) {
            scheduled_intervention->addIntervention(intervention);
        }
        RepastProcess::instance()->getScheduleRunner().scheduleEvent(intervention_at - 0.001, 
            scheduled_intervention);
    }
}

ARTLagCalculator create_art_lag_calc() {
    DayRangeCalculatorCreator creator;
    vector<string> lag_keys;
    Parameters::instance()->getKeys(ART_LAG_PREFIX_LT, lag_keys);
    for (auto& key : lag_keys) {
        std::cout << key << ": " << Parameters::instance()->getStringParameter(key) << std::endl;
        creator.addBin(Parameters::instance()->getStringParameter(key));
    }
    std::shared_ptr<DayRangeCalculator> lower = creator.createCalculator();

    creator.clear();
    lag_keys.clear();
    Parameters::instance()->getKeys(ART_LAG_PREFIX_GTE, lag_keys);
    for (auto& key : lag_keys) {
        std::cout << key << ": " << Parameters::instance()->getStringParameter(key) << std::endl;
        creator.addBin(Parameters::instance()->getStringParameter(key));
    }
    std::shared_ptr<DayRangeCalculator> upper = creator.createCalculator();

    float age_threshold =  Parameters::instance()->getFloatParameter(INPUT_AGE_THRESHOLD);
    return ARTLagCalculator(upper, lower, age_threshold);
}



void add_condom_use_prob(CondomUseAssignerFactory& factory, PartnershipType ptype, int network_type,
        const std::string& category_param, const std::string& use_param) {
    double cat_prob = Parameters::instance()->getDoubleParameter(category_param);
    double use_prob = Parameters::instance()->getDoubleParameter(use_param);
    factory.addProbability(ptype, network_type, cat_prob, use_prob);
}

CondomUseAssigner create_condom_use_assigner() {
    CondomUseAssignerFactory factory;
    add_condom_use_prob(factory, PartnershipType::SERODISCORDANT, STEADY_NETWORK_TYPE, SD_STEADY_NEVER_USE_CONDOMS,
            SD_STEADY_NEVER_USE_CONDOMS_PROB);
    add_condom_use_prob(factory, PartnershipType::SERODISCORDANT, STEADY_NETWORK_TYPE, SD_STEADY_RARELY_USE_CONDOMS,
            SD_STEADY_RARELY_USE_CONDOMS_PROB);
    add_condom_use_prob(factory, PartnershipType::SERODISCORDANT, STEADY_NETWORK_TYPE, SD_STEADY_SOMETIMES_USE_CONDOMS,
            SD_STEADY_SOMETIMES_USE_CONDOMS_PROB);
    add_condom_use_prob(factory, PartnershipType::SERODISCORDANT, STEADY_NETWORK_TYPE, SD_STEADY_USUALLY_USE_CONDOMS,
            SD_STEADY_USUALLY_USE_CONDOMS_PROB);
    add_condom_use_prob(factory, PartnershipType::SERODISCORDANT, STEADY_NETWORK_TYPE, SD_STEADY_ALWAYS_USE_CONDOMS,
            SD_STEADY_ALWAYS_USE_CONDOMS_PROB);

    add_condom_use_prob(factory, PartnershipType::SERODISCORDANT, CASUAL_NETWORK_TYPE, SD_CASUAL_NEVER_USE_CONDOMS,
            SD_CASUAL_NEVER_USE_CONDOMS_PROB);
    add_condom_use_prob(factory, PartnershipType::SERODISCORDANT, CASUAL_NETWORK_TYPE, SD_CASUAL_RARELY_USE_CONDOMS,
            SD_CASUAL_RARELY_USE_CONDOMS_PROB);
    add_condom_use_prob(factory, PartnershipType::SERODISCORDANT, CASUAL_NETWORK_TYPE, SD_CASUAL_SOMETIMES_USE_CONDOMS,
            SD_CASUAL_SOMETIMES_USE_CONDOMS_PROB);
    add_condom_use_prob(factory, PartnershipType::SERODISCORDANT, CASUAL_NETWORK_TYPE, SD_CASUAL_USUALLY_USE_CONDOMS,
            SD_CASUAL_USUALLY_USE_CONDOMS_PROB);
    add_condom_use_prob(factory, PartnershipType::SERODISCORDANT, CASUAL_NETWORK_TYPE, SD_CASUAL_ALWAYS_USE_CONDOMS,
            SD_CASUAL_ALWAYS_USE_CONDOMS_PROB);

    add_condom_use_prob(factory, PartnershipType::SEROCONCORDANT, STEADY_NETWORK_TYPE, SC_STEADY_NEVER_USE_CONDOMS,
            SC_STEADY_NEVER_USE_CONDOMS_PROB);
    add_condom_use_prob(factory, PartnershipType::SEROCONCORDANT, STEADY_NETWORK_TYPE, SC_STEADY_RARELY_USE_CONDOMS,
            SC_STEADY_RARELY_USE_CONDOMS_PROB);
    add_condom_use_prob(factory, PartnershipType::SEROCONCORDANT, STEADY_NETWORK_TYPE, SC_STEADY_SOMETIMES_USE_CONDOMS,
            SC_STEADY_SOMETIMES_USE_CONDOMS_PROB);
    add_condom_use_prob(factory, PartnershipType::SEROCONCORDANT, STEADY_NETWORK_TYPE, SC_STEADY_USUALLY_USE_CONDOMS,
            SC_STEADY_USUALLY_USE_CONDOMS_PROB);
    add_condom_use_prob(factory, PartnershipType::SEROCONCORDANT, STEADY_NETWORK_TYPE, SC_STEADY_ALWAYS_USE_CONDOMS,
            SC_STEADY_ALWAYS_USE_CONDOMS_PROB);

    add_condom_use_prob(factory, PartnershipType::SEROCONCORDANT, CASUAL_NETWORK_TYPE, SC_CASUAL_NEVER_USE_CONDOMS,
            SC_CASUAL_NEVER_USE_CONDOMS_PROB);
    add_condom_use_prob(factory, PartnershipType::SEROCONCORDANT, CASUAL_NETWORK_TYPE, SC_CASUAL_RARELY_USE_CONDOMS,
            SC_CASUAL_RARELY_USE_CONDOMS_PROB);
    add_condom_use_prob(factory, PartnershipType::SEROCONCORDANT, CASUAL_NETWORK_TYPE, SC_CASUAL_SOMETIMES_USE_CONDOMS,
            SC_CASUAL_SOMETIMES_USE_CONDOMS_PROB);
    add_condom_use_prob(factory, PartnershipType::SEROCONCORDANT, CASUAL_NETWORK_TYPE, SC_CASUAL_USUALLY_USE_CONDOMS,
            SC_CASUAL_USUALLY_USE_CONDOMS_PROB);
    add_condom_use_prob(factory, PartnershipType::SEROCONCORDANT, CASUAL_NETWORK_TYPE, SC_CASUAL_ALWAYS_USE_CONDOMS,
            SC_CASUAL_ALWAYS_USE_CONDOMS_PROB);

    return factory.createAssigner();
}

RangeWithProbability create_ASM_runner() {
    RangeWithProbabilityCreator creator;
    vector<string> keys;
    Parameters::instance()->getKeys(ASM_PREFIX, keys);
    for (auto& key : keys) {
        creator.addBin(key, Parameters::instance()->getDoubleParameter(key));
    }
    return creator.createRangeWithProbability();
}

RangeWithProbability create_cd4m_runner(const std::string& prefix) {
    RangeWithProbabilityCreator creator;
    vector<string> keys;
    Parameters::instance()->getKeys(prefix, keys);
    for (auto& key : keys) {
        creator.addBin(key, Parameters::instance()->getDoubleParameter(key));
    }
    return creator.createRangeWithProbability();
}

// FileOutput debug_out(unique_file_name("./art_counts.csv"));

// void debug(double tick, Network<Person>& net) {
//     float art_count = 0, inf_count = 0;
//     for (auto iter = net.verticesBegin(); iter != net.verticesEnd(); ++iter) {
//         PersonPtr person = (*iter);
//         if (person->isOnART()) {
//             ++art_count;
//         } 
//         if (person->isInfected()) {
//             ++inf_count;
//         }
//     }
//     //std::cout << "ART Count: " << art_count << ", Inf Count: " << inf_count << std::endl;
//     //std::cout << "On ART: " << (art_count / inf_count) << std::endl;
//     debug_out << tick << ", " << art_count << "," << inf_count << "," << (art_count / inf_count) << std::endl;
//     debug_out.flush();
// }

Model::Model(shared_ptr<RInside>& ri, const std::string& net_var, const std::string& cas_net_var) :
        R(ri), net(false), population{}, trans_runner(create_transmission_runner()), cd4_calculator(create_CD4Calculator()), viral_load_calculator(
                create_ViralLoadCalculator()), viral_load_slope_calculator(create_ViralLoadSlopeCalculator()), current_pop_size {
                0 }, previous_pop_size { 0 }, stage_map { }, persons_to_log { }, trans_params { }, art_lag_calculator {
                create_art_lag_calc() },  
                jail(&net, JailInfRateCalculator(Parameters::instance()->getIntParameter(JAIL_INFECTION_RATE_WINDOW_SIZE), 
                    Parameters::instance()->getDoubleParameter(JAIL_INFECTION_RATE_MULTIPLIER), 
                    Parameters::instance()->getDoubleParameter(JAIL_INFECTION_RATE_DEFAULT))),
                person_creator { trans_runner, Parameters::instance()->getDoubleParameter(DETECTION_WINDOW), art_lag_calculator, &jail}, prep_manager(),	
                condom_assigner { create_condom_use_assigner() },
                asm_runner { create_ASM_runner() },  cd4m_treated_runner{ create_cd4m_runner(CD4M_TREATED_PREFIX)}, 
                age_threshold{Parameters::instance()->getFloatParameter(INPUT_AGE_THRESHOLD)}
                 {

    std::cout << "treated: " << cd4m_treated_runner << std::endl;
    // get initial stats
    init_stats();
    init_trans_params(trans_params);
    init_logs();
    initialize_prep_interventions(prep_manager);

    List rnet = as<List>((*R)[net_var]);
    initialize_network(rnet, net, person_creator, condom_assigner, STEADY_NETWORK_TYPE);
    rnet = as<List>((*R)[cas_net_var]);
    initialize_edges(rnet, net, condom_assigner, CASUAL_NETWORK_TYPE);

    population.insert(population.end(), net.verticesBegin(), net.verticesEnd());
    initialize_jail(jail, rnet, population);

    init_stage_map(stage_map);
    init_network_save(&jail, &net, R);

    current_pop_size = population.size();

    init_biomarker_logging(current_pop_size, persons_to_log);
    Stats* stats = TransModel::Stats::instance();
    stats->currentCounts().main_edge_count = net.edgeCount(STEADY_NETWORK_TYPE);
    stats->currentCounts().casual_edge_count = net.edgeCount(CASUAL_NETWORK_TYPE);
    for (auto iter = population.begin(); iter != population.end(); ++iter) {
        PersonPtr p = *iter;
        stats->personDataRecorder()->initRecord(p, 0);
        if (p->isInfected()) {
            stats->currentCounts().incrementInfected(p);
            stats->personDataRecorder()->recordInfection(p, p->infectionParameters().time_of_infection,
                    InfectionSource::INTERNAL);
        }
    }
    stats->resetForNextTimeStep();

    init_generators();
    

    ScheduleRunner& runner = RepastProcess::instance()->getScheduleRunner();
    runner.scheduleStop(Parameters::instance()->getDoubleParameter("stop.at"));
    runner.scheduleEvent(1, 1, Schedule::FunctorPtr(new MethodFunctor<Model>(this, &Model::step)));
    runner.scheduleEndEvent(Schedule::FunctorPtr(new MethodFunctor<Model>(this, &Model::atEnd)));
    runner.scheduleEvent(364.9, 365, Schedule::FunctorPtr(new MethodFunctor<PrepInterventionManager>(&prep_manager, &PrepInterventionManager::onYearEnded)));

    initPrepCessation();
    schedule_jail_interventions(runner, this, &jail);
    //write_edges(net, "./edges_at_1.csv");
}

void Model::initPrepCessation() {
    ScheduleRunner& runner = RepastProcess::instance()->getScheduleRunner();
    for (auto iter = population.begin(); iter != population.end(); ++iter) {
        PersonPtr person = *iter;
        if (person->isOnPrep(false)) {
            double stop_time = person->prepParameters().stopTime();
            runner.scheduleEvent(stop_time, Schedule::FunctorPtr(new PrepCessationEvent(person, stop_time)));
            double start_time = person->prepParameters().startTime();
            Stats::instance()->recordPREPEvent(start_time, person->id(), static_cast<int>(PrepStatus::ON));
            Stats::instance()->personDataRecorder()->recordPREPStart(person, start_time);
        }
    }
}

void Model::atEnd() {
    double ts = RepastProcess::instance()->getScheduleRunner().currentTick();
    std::shared_ptr<PersonDataRecorderI> pdr = Stats::instance()->personDataRecorder();
    for (auto iter = population.begin(); iter != population.end(); ++iter) {
        pdr->finalize(*iter, ts);
    }

    // forces stat writing via destructors
    delete Stats::instance();
    delete Logger::instance();

    //write_edges(net, "./edges_at_end.csv");
}

Model::~Model() {
}

void Model::updateThetaForm(const std::string& var_name) {
    NumericVector theta_form = as<NumericVector>((*R)[var_name]);
    theta_form[0] = theta_form[0] + std::log(previous_pop_size) - std::log(current_pop_size);
    ((*R)[var_name]) = theta_form;
}

void Model::countOverlap() {
    int target_type = 0;
    int other_type = 0;
    if (net.edgeCount(STEADY_NETWORK_TYPE) < net.edgeCount(CASUAL_NETWORK_TYPE)) {
        target_type = STEADY_NETWORK_TYPE;
        other_type = CASUAL_NETWORK_TYPE;
    } else {
        target_type = CASUAL_NETWORK_TYPE;
        other_type = STEADY_NETWORK_TYPE;
    }

    Stats* stats = Stats::instance();
    for (auto iter = net.edgesBegin(); iter != net.edgesEnd(); ++iter) {
        if ((*iter)->type() == target_type) {
            EdgePtr<Person> edge = (*iter);
            if (net.hasEdge(edge->v1(), edge->v2(), other_type) || net.hasEdge(edge->v2(), edge->v1(), other_type)) {
                ++(stats->currentCounts().overlaps);
            }
        }
    }
}

void Model::step() {
    double t = RepastProcess::instance()->getScheduleRunner().currentTick();
    Stats* stats = Stats::instance();
    stats->currentCounts().tick = t;

    // PersonToVAL: a function that translates our Person objects into 
    // "Vertex Attribute Lists" -- R's statnet network vertex representation
    PersonToVALForSimulate p2val;
    float max_survival = Parameters::instance()->getFloatParameter(MAX_AGE);
    float size_of_timestep = Parameters::instance()->getIntParameter(SIZE_OF_TIMESTEP);
       
    if ((int) t % 100 == 0) {
        std::cout << " ---- " << t << " ---- " << std::endl;
    }
   
    // updates (i.e. simulates) the partner network
    simulate(R, net, p2val, condom_assigner, t);

    if (Parameters::instance()->getBooleanParameter(COUNT_OVERLAPS)) {
        countOverlap();
    } else {
        stats->currentCounts().overlaps = -1;
    }

    // introduce new persons into the model
    entries(t, size_of_timestep);
    // run the HIV transmission algorithm 
    unsigned int uninfected_count = uninfectedPopulationSize();
    unsigned int infected_count = runTransmission(t);
    vector<PersonPtr> uninfected;

    // update the physiological etc. (vital) attributes of the population
    // updating cd4 counts, checking for death, diagnosing persons, etc.
    updateVitals(t, size_of_timestep, max_survival, uninfected);
    // select members of the population for infection from external sources
    runExternalInfections(uninfected, t);
    jail.addOutsideInfectionRate(infected_count, uninfected_count);
    runJailInfections(t);
    previous_pop_size = current_pop_size;
    current_pop_size = population.size();
    //std::cout << "tick= " << t << ", current_pop_size: "<< current_pop_size<<std::endl; 


    // the R statnet network update code needs these updated every iteration
    updateThetaForm("theta.form");
    updateThetaForm("theta.form_cas");

    stats->currentCounts().pop = current_pop_size;
    
    stats->currentCounts().main_edge_count = net.edgeCount(STEADY_NETWORK_TYPE);
    stats->currentCounts().casual_edge_count = net.edgeCount(CASUAL_NETWORK_TYPE);

    for (auto iter = population.begin(); iter != population.end(); ++iter) {
        PersonPtr person = (*iter);
        stats->currentCounts().incrementVertexCount(person);
        if (!person->isInfected()) {
            stats->currentCounts().incrementUninfected(person);
        }
    }

    stats->resetForNextTimeStep();

    //new_infected_this_cycle=0;

}

void Model::schedulePostDiagnosisART(PersonPtr person, std::map<double, ARTScheduler*>& art_map, double tick,
        float size_of_timestep) {
    double lag = art_lag_calculator.calculateLag(person, size_of_timestep);
    Stats::instance()->personDataRecorder()->recordInitialARTLag(person, lag);
    person->setARTLag(lag);

    double art_at_tick = lag + tick;
    ARTScheduler* scheduler = nullptr;
    auto iter = art_map.find(art_at_tick);
    //std::cout << "-----tick: " << tick<< ", lag: "<< lag<< std::endl;

    if (iter == art_map.end()) {
        //std::cout << " iter==map.end: " << iter->first << std::endl;

        scheduler = new ARTScheduler((float) art_at_tick);
        RepastProcess::instance()->getScheduleRunner().scheduleEvent(art_at_tick - 0.1,
                repast::Schedule::FunctorPtr(scheduler));
        art_map.emplace(art_at_tick, scheduler);
    } else {
        scheduler = iter->second;
    }
    scheduler->addPerson(person);
}

void Model::updateDisease(PersonPtr person) {
    if (person->isOnART(true)) { //care disruption mechanism
    //if (person->isOnART()) { 
        float slope = viral_load_slope_calculator.calculateSlope(person->infectionParameters());
        person->setViralLoadARTSlope(slope);
    }

    float viral_load = viral_load_calculator.calculateViralLoad(person->infectionParameters());
    person->setViralLoad(viral_load);
    // update cd4
    float cd4 = cd4_calculator.calculateCD4(person->age(), person->infectionParameters());
    person->setCD4Count(cd4);

    // select stage, and use it
    float infectivity = stage_map.upper_bound(person->timeSinceInfection())->second->calculateInfectivity(
            person->infectionParameters());
    person->setInfectivity(infectivity);
}

void Model::updateJailStats(Stats* stats, PersonPtr person) {

    std::vector<EdgePtr<Person>> edges;
    net.getEdges(person, edges);
    for (auto edge : edges) { 
        if (edge->v1()->id() != person->id()) {
            if (edge->v1()->isInfected())
                ++stats->currentCounts().infected_partners_at_incarceration;
            }
        if (edge->v2()->id() != person->id()) {
            if (edge->v2()->isInfected())
                 ++stats->currentCounts().infected_partners_at_incarceration;
        }
    }

    if (person->isInfected()) {
        ++stats->currentCounts().infected_at_incarceration;
    }

    ++stats->currentCounts().incarcerated;

    if (person->hasPreviousJailHistory()) {
        ++stats->currentCounts().incarcerated_recidivist;
    }
}
    
    
void Model::doJailCheck(PersonPtr person, double tick, double incarceration_with_cji_prob, double incarceration_prob) {
    //test only
    //if (person->isInfected()) {} //only HIV+
    //proability of jailing a person: 
    if (!person->isJailed())  {
        if (person->hasPreviousJailHistory()) {
            if (Random::instance()->nextDouble() <=  incarceration_with_cji_prob) {
                updateJailStats(Stats::instance(), person);
                jail.addPerson(tick, person);
            }
        } else if (Random::instance()->nextDouble() <= incarceration_prob) {
            updateJailStats(Stats::instance(), person);
            jail.addPerson(tick, person);
        }
    }
}

void Model::updateVitals(double tick, float size_of_timestep, int max_age, vector<PersonPtr>& uninfected) {
    unsigned int dead_count = 0;
    
    Stats* stats = Stats::instance();
    map<double, ARTScheduler*> art_map;

    uninfected.reserve(population.size());

    float time_to_full_supp = Parameters::instance()->getFloatParameter(TIME_TO_FULL_SUPP);
    int vs_count = 0, inf_count = 0, diagnosed_count = 0, vs_pos_count = 0;

    double incarceration_prob = Parameters::instance()->getDoubleParameter(INCARCERATION_PROB);
    //double incarceration_prob_prev = Parameters::instance()->getDoubleParameter(INCARCERATION_PROB_PREV);
    double incarceration_with_cji_prob = Parameters::instance()->getDoubleParameter(INCARCERATION_WITH_CJI_PROB);

    for (auto iter = population.begin(); iter != population.end(); ) {
        PersonPtr person = (*iter);
        // update viral load, cd4
        if (person->isInfected()) {
            updateDisease(person);
        }

        if (persons_to_log.find(person->id()) != persons_to_log.end()) {
            stats->recordBiomarker(tick, person);
        }

        // diagnose person and schedule for ART 
        if (person->isTestable() && !person->isDiagnosed()) {
            if (person->diagnose(tick)) {
                schedulePostDiagnosisART(person, art_map, tick, size_of_timestep);
            }
        }

        bool crossed_thresh = person->step(size_of_timestep, age_threshold);
        // check person for "death" -- aging out, death by infection.
        CauseOfDeath cod = dead(tick, person, max_age);
        if (cod != CauseOfDeath::NONE) {
            // person died -- remove them from model, network, etc.
            vector<EdgePtr<Person>> edges;
            PartnershipEvent::PEventType pevent_type = cod_to_PEvent(cod);
            net.getEdges(person, edges);
            for (auto edge : edges) {
                //cout << edge->id() << "," << static_cast<int>(cod) << "," << static_cast<int>(pevent_type) << endl;
                Stats::instance()->recordPartnershipEvent(tick, edge->id(), edge->v1()->id(), edge->v2()->id(),
                        pevent_type, edge->type());
            }
            net.removeVertex(person);
            iter = population.erase(iter);
            if (person->isJailed()) {
	            jail.removeDeadPerson(tick, person);
            }
            ++dead_count;
        } else {
            // don't count dead uninfected persons
            if (!person->isInfected()) {
                // accumulate persons who may potentially go on PrEP
                prep_manager.processPerson(person, net);
                if (person->isOnPrep(true)) {  //care disruption
                //if (person->isOnPrep()) {
                    ++stats->currentCounts().on_prep;
                }
                uninfected.push_back(person);

            } else {

                if ( person->infectionParameters().time_since_infection >= time_to_full_supp) {
                    ++inf_count;
                    if (person->infectionParameters().viral_load < VS_VL_COUNT) {
                        ++vs_count;
                    }
                }

                if (person->isDiagnosed() && !isnan(person->infectionParameters().time_since_infection) &&
                                person->infectionParameters().time_since_infection >= time_to_full_supp) {
                    ++diagnosed_count;
                    if (person->infectionParameters().viral_load < VS_VL_COUNT) {
                        ++vs_pos_count;
                    }
                }
            }
            
            doJailCheck(person, tick, incarceration_with_cji_prob, incarceration_prob);

            if (person->isOnART(true)) { //care disruption 
            // if (person->isOnART()) {
                ++stats->currentCounts().on_art;
            }

            if (crossed_thresh) {
                // crossed_thresh means Person crossed from less than to greater than equal
                // age category so testing and adherece parameters need to be updated to
                // reflect that
                person_creator.updateTesting(person, size_of_timestep);
                person_creator.updatePREPAdherence(person);
            }
            ++iter;
        }
    }

    stats->currentCounts().vl_supp_per_positives = inf_count == 0 ? 0 : ((double)vs_count) / inf_count;
    stats->currentCounts().vl_supp_per_diagnosis = diagnosed_count == 0 ? 0 : ((double)vs_pos_count) / diagnosed_count;
    // Select from accumulated potential persons those to put on PrEP.
    prep_manager.run(tick, net);
    // Reset the prep uptake for next round
    prep_manager.reset();
}

unsigned int Model::runExternalInfections(vector<PersonPtr>& uninfected, double t) {

    double min = Parameters::instance()->getDoubleParameter(EXTERNAL_INFECTION_RATE_MIN);
    double max = Parameters::instance()->getDoubleParameter(EXTERNAL_INFECTION_RATE_MAX);
    double val = Random::instance()->createUniDoubleGenerator(min, max).next();

    double prob = uninfected.size() * val;

    if (Random::instance()->nextDouble() <= prob) {
        float min_age = Parameters::instance()->getFloatParameter(MIN_AGE);
        float factor = Parameters::instance()->getFloatParameter(EXTERNAL_INFECTION_AGE_FACTOR);

        std::map<float, PersonPtr> prob_map;
        float sum = 0;
        for (auto p : uninfected) {
            int exp = ((int) floor(p->age() - min_age));
            sum += (float) pow(factor, exp);
            prob_map.emplace(sum, p);

        }

        float draw = (float) Random::instance()->createUniDoubleGenerator(0, sum).next();
        auto iter = prob_map.lower_bound(draw);

        Stats* stats = Stats::instance();
        PersonPtr p = iter->second;
        infectPerson(p, t);
        stats->currentCounts().incrementInfectedExternal(p);
        stats->personDataRecorder()->recordInfection(p, t, InfectionSource::EXTERNAL);
        return 1;
    }
    return 0;
}

void Model::infectPerson(PersonPtr& person, double time_stamp) {
    trans_runner->infect(person, time_stamp);

    vector<EdgePtr<Person>> edges;
    net.getEdges(person, edges);
    for (EdgePtr<Person> ptr : edges) {
        condom_assigner.initEdge(ptr);
    }
}

void Model::entries(double tick, float size_of_timestep) {
    float min_age = Parameters::instance()->getFloatParameter(MIN_AGE);
    size_t pop_size = population.size();
    if (pop_size > 0) {
        double births_prob = Parameters::instance()->getDoubleParameter(DAILY_ENTRY_RATE);
        PoissonGen birth_gen(Random::instance()->engine(), boost::random::poisson_distribution<>(births_prob));
        DefaultNumberGenerator<PoissonGen> gen(birth_gen);
        int entries = (int) gen.next();
        Stats* stats = Stats::instance();
        stats->currentCounts().entries = entries;

        double infected_prob = Parameters::instance()->getDoubleParameter(INIT_HIV_PREV_ENTRIES);

        for (int i = 0; i < entries; ++i) {
            VertexPtr<Person> p = person_creator(tick, min_age);
            if (Random::instance()->nextDouble() <= infected_prob) {
                // as if infected at previous timestep
                float infected_at = tick - (size_of_timestep * 1);
                trans_runner->infect(p, infected_at);
                float viral_load = viral_load_calculator.calculateViralLoad(p->infectionParameters());
                p->setViralLoad(viral_load);
                // update cd4
                float cd4 = cd4_calculator.calculateCD4(p->age(), p->infectionParameters());
                p->setCD4Count(cd4);
                stats->currentCounts().incrementInfectedAtEntry(p);
                stats->recordInfectionEvent(infected_at, p);
            }
            population.push_back(p);
            net.addVertex(p);
            Stats::instance()->personDataRecorder()->initRecord(p, tick);
        }
    }
}

CauseOfDeath Model::dead(double tick, PersonPtr person, int max_age) {
    int death_count = 0;
    CauseOfDeath cod = CauseOfDeath::NONE;
    // dead of old age
    if (person->deadOfAge(max_age)) {
        ++death_count;
        ++Stats::instance()->currentCounts().age_deaths;
        Stats::instance()->recordDeathEvent(tick, person, DeathEvent::AGE);
        Stats::instance()->personDataRecorder()->recordDeath(person, tick);
        cod = CauseOfDeath::AGE;
    }

    if (cod == CauseOfDeath::NONE && person->deadOfInfection()) {
        // infection deaths
        ++death_count;
        ++Stats::instance()->currentCounts().infection_deaths;
        Stats::instance()->recordDeathEvent(tick, person, DeathEvent::INFECTION);
        Stats::instance()->personDataRecorder()->recordDeath(person, tick);
        cod = CauseOfDeath::INFECTION;
    }

    if (cod == CauseOfDeath::NONE) { 
        double increase = 0;
        if (person->isOnART(true)) { //care disruption
        //if (person->isOnART()) {
            increase = cd4m_treated_runner.lookup(person->infectionParameters().cd4_count);
        }

        if (asm_runner.run(person->age(), increase, Random::instance()->nextDouble())) {
            // asm deaths
            ++death_count;
            Stats::instance()->personDataRecorder()->recordDeath(person, tick);
            if (increase > 0) {
                ++Stats::instance()->currentCounts().cd4m_deaths;
                Stats::instance()->recordDeathEvent(tick, person, DeathEvent::ASM_CD4);
                cod = CauseOfDeath::ASM_CD4;
            } else {
                ++Stats::instance()->currentCounts().asm_deaths;
                Stats::instance()->recordDeathEvent(tick, person, DeathEvent::ASM);
                cod = CauseOfDeath::ASM;
            }
        } 
    }

    person->setDead(cod != CauseOfDeath::NONE);
    return cod;
}

bool Model::hasSex(int edge_type) {
    double prob;
    if (edge_type == STEADY_NETWORK_TYPE) {
        prob = trans_params.prop_steady_sex_acts;
    } else {
        prob = trans_params.prop_casual_sex_acts;
    }

    return Random::instance()->nextDouble() <= prob;
}

void record_sex_act(int edge_type, bool condom_used, bool discordant, Stats* stats) {
    ++stats->currentCounts().sex_acts;
    if (edge_type == STEADY_NETWORK_TYPE) {

        ++stats->currentCounts().steady_sex_acts;
        if (condom_used) {
            if (discordant) {
                ++stats->currentCounts().sd_steady_sex_with_condom;
            } else {
                ++stats->currentCounts().sc_steady_sex_with_condom;
            }
        } else {
            if (discordant) {
                ++stats->currentCounts().sd_steady_sex_without_condom;
            } else {
                ++stats->currentCounts().sc_steady_sex_without_condom;
            }
        }
    } else {
        ++stats->currentCounts().casual_sex_acts;
        if (condom_used) {
            if (discordant) {
                ++stats->currentCounts().sd_casual_sex_with_condom;
            } else {
                ++stats->currentCounts().sc_casual_sex_with_condom;
            }
        } else {
            if (discordant) {
                ++stats->currentCounts().sd_casual_sex_without_condom;
            } else {
                ++stats->currentCounts().sc_casual_sex_without_condom;
            }
        }
    }
}

unsigned int Model::runTransmission(double time_stamp) {
    vector<PersonPtr> infecteds;

    Stats* stats = Stats::instance();
    for (auto iter = net.edgesBegin(); iter != net.edgesEnd(); ++iter) {
        int type = (*iter)->type();
        if (hasSex(type)) {
            bool condom_used = (*iter)->useCondom(Random::instance()->nextDouble());

            bool discordant = false;
            PersonPtr out_p = (*iter)->v1();
            PersonPtr in_p = (*iter)->v2();

            if (out_p->isInfected() && !in_p->isInfected()) {
                discordant = true;

                if (trans_runner->determineInfection(out_p, in_p, condom_used, type)) {
                    infecteds.push_back(in_p);
                    Stats::instance()->recordInfectionEvent(time_stamp, out_p, in_p, false, (*iter)->type());
                }
            } else if (!out_p->isInfected() && in_p->isInfected()) {
                discordant = true;

                if (trans_runner->determineInfection(in_p, out_p, condom_used, type)) {
                    infecteds.push_back(out_p);
                    Stats::instance()->recordInfectionEvent(time_stamp, in_p, out_p, false, (*iter)->type());
                }
            }
            record_sex_act(type, condom_used, discordant, stats);
        }
    }

    unsigned int new_infected_count = 0;
    for (auto& person : infecteds) {
        // if person has multiple partners who are infected,
        // person gets multiple chances to become infected from them
        // and so may appear more than once in the infecteds list
        if (!person->isInfected()) {
            infectPerson(person, time_stamp);
            stats->currentCounts().incrementInfected(person);
            stats->personDataRecorder()->recordInfection(person, time_stamp, InfectionSource::INTERNAL);
            stats->currentCounts().incrementNewlyInfected();   //infected after burnin     
            ++new_infected_count;
        }
    }

    return new_infected_count;
}

void Model::runJailInfections(double time_stamp) {
    Stats* stats = Stats::instance();
    std::vector<PersonPtr> infected_in_jail;
    std::vector<EdgePtr<Person>> infected_edges;
    jail.runInternalInfectionTransmission(time_stamp, infected_in_jail, infected_edges);
    for (auto& person : infected_in_jail) {
        infectPerson(person, time_stamp);
        stats->currentCounts().incrementInfected(person); 
        stats->personDataRecorder()->recordInfection(person, time_stamp, InfectionSource::INJAIL);
        stats->currentCounts().incrementNewlyInfected();
    } 

    for (auto& edge: infected_edges) {
        condom_assigner.initEdge(edge);
    }
}

int Model::infectedPopulationSize() {
    float totalInfectedPop=0;
    for (auto& p : population) {
        if (!p->isDead()) {
            if (p->isInfected()) {
                totalInfectedPop++;
            }
        }
    }
    return totalInfectedPop;
}

int Model::uninfectedPopulationSize() {
    float totaluninfectedPop=0;
    for (auto& p : population) {
        if (!p->isDead() && !p->isInfected()) {
            ++totaluninfectedPop;
        }
    }
    return totaluninfectedPop;
}


/**
* Function to calcuate infection incidence 
* When neeeded, the function is only used to calcuate infection rate over a certain period (e.g. year) in order to
* to set the infection incidence rate (per day) inside the jail (half of the general population)
*/ 
float Model::infectionIncidence() { 
    Stats* stats = Stats::instance();
    //return (float) infectedPopulationSize()/(float)uninfectedPopulationSize(); //infection prevalence

   //number 
   int int_infected = std::accumulate(stats->currentCounts().internal_infected.begin(), stats->currentCounts().internal_infected.end(), 0);
   int ext_infected = std::accumulate(stats->currentCounts().external_infected.begin(), stats->currentCounts().external_infected.end(), 0);
   int inf_at_entry = std::accumulate(stats->currentCounts().infected_at_entry.begin(), stats->currentCounts().infected_at_entry.end(), 0);
   int inf_in_jail = stats->currentCounts().infected_inside_jail;
   int uninfect = std::accumulate(stats->currentCounts().uninfected.begin(), stats->currentCounts().uninfected.end(), 0);
   
   int total_infected_this_cycle = int_infected + ext_infected + inf_at_entry + inf_in_jail; 
   //return (float) stats->currentCounts().total_internal_infected_new /(float)uninfectedPopulationSize();
   return (float) total_infected_this_cycle/(float)uninfect;
}

/**
* Function to calcuate infection incidence based on person-day 
* When neeeded, the function is only used to calcuate infection rate over a certain period (e.g. year) in order to
* to set the infection incidence rate (per day) inside the jail (half of the general population)
*/ 
float Model::infectionIncidence_personDay(double time) {
    Stats* stats = Stats::instance();
    int infected_today = std::accumulate(stats->currentCounts().internal_infected.begin(), stats->currentCounts().internal_infected.end(), 0);
    total_infected_person_days += infected_today * time;
    return (float) infectedPopulationSize()/(float) (uninfectedPopulationSize() + total_infected_person_days);
}


} /* namespace TransModel */
