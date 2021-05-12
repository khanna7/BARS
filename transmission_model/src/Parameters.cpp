/*
 * Parameters.cpp
 *
 *  Created on: Jun 5, 2012
 *      Author: nick
 */

#include "boost/algorithm/string.hpp"

#include "repast_hpc/Utilities.h"
#include "repast_hpc/Properties.h"

#include "Parameters.h"

using namespace repast;


namespace TransModel {

const std::string RUN_NUMBER = "run";

const std::string INPUT_AGE_THRESHOLD = "input.age.threshold";

const std::string LT_SUFFIX = ".lt";
const std::string GTE_SUFFIX = ".gte";

const std::string METH_SUFFIX = ".meth";
const std::string CRACK_SUFFIX = ".crack";
const std::string ECSTASY_SUFFIX = ".ecstasy";

const std::string NET_VAR = "net.variable.name";
const std::string CASUAL_NET_VAR = "casual.net.variable.name";

const std::string STOP_AT = "stop.at";
const std::string R_PARAMETERS_DERIVED = "r.parameters.derived";
const std::string R_PARAMETERS_NON_DERIVED = "r.parameters.non.derived";
const std::string R_FILE = "r.file";
const std::string INIT_DATA_FILE = "initial.data.file";
const std::string MAIN_NETWORK_FILE = "main.network.file";
const std::string CASUAL_NETWORK_FILE = "casual.network.file";
const std::string EVENT_FILE = "event.file";
const std::string EVENT_FILE_BUFFER_SIZE = "event.file.buffer.size";
const std::string COUNTS_PER_TIMESTEP_OUTPUT_FILE= "per.tick.counts.output.file";
const std::string OUTPUT_DIR = "output.directory";
const std::string PARTNERSHIP_EVENTS_FILE = "partnership.events.file";
const std::string INFECTION_EVENTS_FILE =  "infection.events.file";
const std::string BIOMARKER_FILE = "biomarker.log.file";
const std::string BIOMARKER_LOG_COUNT = "biomarker.persons.to.log";
const std::string DEATH_EVENT_FILE = "death.events.file";
const std::string TESTING_EVENT_FILE = "testing.events.file";
const std::string ART_EVENT_FILE = "art.events.file";
const std::string PREP_EVENT_FILE = "prep.events.file";
const std::string PERSON_DATA_FILE = "person.data.file";
const std::string NET_SAVE_FILE = "net.save.file";
const std::string CASUAL_NET_SAVE_FILE = "casual.net.save.file";
const std::string NET_SAVE_AT = "save.network.at";
const std::string COUNT_OVERLAPS = "count.overlaps";

const std::string SERO_LOG_FILE = "serodiscordant.prep.log.file";
const std::string BASE_LOG_FILE = "default.prep.log.file";
const std::string NET_LOG_FILE = "network.prep.log.file";
const std::string RANDOM_SELECTION_LOG_FILE = "random.selection.prep.log.file";

//const std::string PREP_MULT = "prep.mult";

// generated from parameters.R
const std::string ACUTE_LENGTH_MIN = "acute.length.min";
const std::string ACUTE_LENGTH_MAX = "acute.length.max";
const std::string ACUTE_MULT = "acute.mult";
const std::string ACUTE_MULT_HOLLING = "acute.mult.holling";
const std::string B1_REF = "b1.ref";
const std::string B2_AFRICAN = "b2.african";
const std::string B3_FEMALE = "b3.female";
const std::string B4_CD4_REF = "b4.cd4.ref";
const std::string B5_AFRICAN = "b5.african";
const std::string B6_AGE_15TO29 = "b6.age.15to29";
const std::string B6_AGE_30TO39 = "b6.age.30to39";
const std::string B6_AGE_40TO49 = "b6.age.40to49";
const std::string B6_AGE_50ORMORE = "b6.age.50ormore";
const std::string CD4_AT_INFECTION_MALE = "cd4.at.infection.male";
const std::string CD4_REC_PER_TIMESTEP = "cd4.rec.per.timestep";
const std::string CD4_RECOVERY_TIME = "cd4.recovery.time";
const std::string CHRONIC_LENGTH_MIN = "chronic.length.min";
const std::string CHRONIC_LENGTH_MAX = "chronic.length.max";
const std::string CIRCUM_MULT = "circum.mult";
const std::string CIRCUM_RATE = "circum.rate";
const std::string IS_COUNSELING_AND_BEHAVIORAL_TREATMENT_ON =
        "is.counseling.and.behavioral.treatment.on";
const std::string COUNSELING_AND_BEHAVIORAL_TREATMENT_USE_PROP =
        "counseling.and.behavioral.treatment.use.prop";
const std::string COUNSELING_AND_BEHAVIORAL_TREATMENT_LENGTH =
        "counseling.and.behavioral.treatment.length";
const std::string COUNSELING_AND_BEHAVIORAL_TREATMENT_ADHERENCE_PROP =
        "counseling.and.behavioral.treatment.adherence.prop";
const std::string DAILY_ENTRY_RATE = "daily.entry.rate";
const std::string DETECTION_WINDOW = "detection.window";
const std::string DUR_INF = "dur.inf";
const std::string DURATION_OF_INFECTION = "duration.of.infection";
const std::string INIT_HIV_PREV = "init.hiv.prev";
const std::string INIT_HIV_PREV_ENTRIES = "init.hiv.prev.for.entries";
const std::string INFECTIVE_INSERTIVE_MULT = "inf.part.insertive.mult";
const std::string LATE_LENGTH_MIN = "late.length.min";
const std::string LATE_LENGTH_MAX = "late.length.max";
const std::string LATE_MULT = "late.mult";
const std::string LATE_MULT_HOLLING = "late.mult.holling";
const std::string LATE_STAGE_VIRAL_LOAD = "late.stage.viral.load";
const std::string MAX_AGE = "max.age";
const std::string MIN_AGE = "min.age";
const std::string MIN_CHRONIC_INFECTIVITY_UNADJ = "min.chronic.infectivity.unadj";
const std::string MIRTAZAPINE_TREATMENT_USE_PROP = "mirtazapine.treatment.use.prop";
const std::string MIRTAZAPINE_TREATMENT_ADHERENCE_PROP = "mirtazapine.treatment.adherence.prop";
const std::string MIRTAZAPINE_TREATMENT_LENGTH = "mirtazapine.treatment.length";
const std::string NON_TESTERS_PROP_LT = "non.testers.prop.lt";
const std::string NON_TESTERS_PROP_GTE = "non.testers.prop.gte";
const std::string NUM_SEX_ACTS_PER_TIMESTEP = "num.sex.acts.per.timestep";
const std::string PEAK_VIRAL_LOAD = "peak.viral.load";
const std::string PER_DAY_CD4_RECOVERY = "per.day.cd4.recovery";
const std::string PREG_MULT = "preg.mult";
const std::string PREG_SUSC_MULT = "preg.susc.mult";

const std::string PREP_SCHEME = "prep.uptake";
const std::string PREP_INTERVENTION_SCHEME = "prep.intervention";
const std::string PREP_INTERVENTION_AT = "prep.intervention.at";

const std::string DEFAULT_PREP_USE_PROP_LT = "default.prep.bl.use.prop.lt";
const std::string DEFAULT_PREP_USE_PROP_GTE = "default.prep.bl.use.prop.gte";
const std::string DEFAULT_PREP_DAILY_STOP_PROB_LT = "default.prep.daily.stop.prob.lt";
const std::string DEFAULT_PREP_DAILY_STOP_PROB_GTE = "default.prep.daily.stop.prob.gte";
const std::string DEFAULT_PREP_YEARLY_INCREMENT_LT = "default.prep.yearly.increment.lt";
const std::string DEFAULT_PREP_YEARLY_INCREMENT_GTE = "default.prep.yearly.increment.gte";
const std::string DEFAULT_PREP_YEARS_TO_INCREMENT = "default.prep.years.to.increment";
const std::string DEFAULT_PREP_BALANCED_UNBALANCED = "default.prep.balanced.unbalanced";
const std::string DEFAULT_PREP_UNBALANCED_STARTING_PROB_LT = "default.prep.unbalanced.starting.prob.lt";
const std::string DEFAULT_PREP_UNBALANCED_STARTING_PROB_GTE = "default.prep.unbalanced.starting.prob.gte";
const std::string DEFAULT_PREP_UNBALANCED_STARTING_PROB = "default.prep.unbalanced.starting.prob";

const std::string DEFAULT_PREP_USE_PROP = "default.prep.bl.use.prop";
const std::string DEFAULT_PREP_DAILY_STOP_PROB = "default.prep.daily.stop.prob";

const std::string YOR_PREP_ALPHA = "yor.prep.alpha";
const std::string YOR_PREP_USE_PROP = "yor.prep.bl.use.prop";
const std::string YOR_PREP_DAILY_STOP_PROB = "yor.prep.daily.stop.prob";
const std::string YOR_PREP_YEARLY_INCREMENT = "yor.prep.yearly.increment";
const std::string YOR_PREP_YEARS_TO_INCREMENT = "yor.prep.years.to.increment";
const std::string YOR_ADDITIONAL_PREP_LT = "yor.prep.additional.lt";
const std::string YOR_ADDITIONAL_PREP_GTE = "yor.prep.additional.gte";

const std::string SERO_BASE_PREP_USE_PROP_LT = "serodiscordant.base.prep.bl.use.prop.lt";
const std::string SERO_BASE_PREP_USE_PROP_GTE = "serodiscordant.base.prep.bl.use.prop.gte";
const std::string SERO_BASE_PREP_DAILY_STOP_PROB_LT = "serodiscordant.base.prep.daily.stop.prob.lt";
const std::string SERO_BASE_PREP_DAILY_STOP_PROB_GTE = "serodiscordant.base.prep.daily.stop.prob.gte";

const std::string SERO_INTRV_PREP_DAILY_STOP_PROB_LT = "serodiscordant.intrv.prep.daily.stop.prob.lt";
const std::string SERO_INTRV_PREP_DAILY_STOP_PROB_GTE = "serodiscordant.intrv.prep.daily.stop.prob.gte";
const std::string SERO_PREP_YEARLY_INCREMENT_LT = "serodiscordant.intrv.prep.yearly.increment.lt";
const std::string SERO_PREP_YEARLY_INCREMENT_GTE = "serodiscordant.intrv.prep.yearly.increment.gte";
const std::string SERO_PREP_YEARS_TO_INCREMENT = "serodiscordant.intrv.prep.years.to.increment";
const std::string SERO_NET_TYPE = "serodiscordant.intrv.prep.network.type";

const std::string EIGEN_BASE_PREP_USE_PROP_LT = "eigen.base.prep.bl.use.prop.lt";
const std::string EIGEN_BASE_PREP_USE_PROP_GTE = "eigen.base.prep.bl.use.prop.gte";
const std::string EIGEN_BASE_PREP_DAILY_STOP_PROB_LT = "eigen.base.prep.daily.stop.prob.lt";
const std::string EIGEN_BASE_PREP_DAILY_STOP_PROB_GTE = "eigen.base.prep.daily.stop.prob.gte";

const std::string EIGEN_INTRV_PREP_DAILY_STOP_PROB_LT = "eigen.intrv.prep.daily.stop.prob.lt";
const std::string EIGEN_INTRV_PREP_DAILY_STOP_PROB_GTE = "eigen.intrv.prep.daily.stop.prob.gte";
const std::string EIGEN_PREP_YEARLY_INCREMENT_LT = "eigen.intrv.prep.yearly.increment.lt";
const std::string EIGEN_PREP_YEARLY_INCREMENT_GTE = "eigen.intrv.prep.yearly.increment.gte";
const std::string EIGEN_PREP_YEARS_TO_INCREMENT = "eigen.intrv.prep.years.to.increment";
const std::string EIGEN_TOPN = "eigen.intrv.prep.topn";

const std::string DEGREE_BASE_PREP_USE_PROP_LT = "degree.base.prep.bl.use.prop.lt";
const std::string DEGREE_BASE_PREP_USE_PROP_GTE = "degree.base.prep.bl.use.prop.gte";
const std::string DEGREE_BASE_PREP_DAILY_STOP_PROB_LT = "degree.base.prep.daily.stop.prob.lt";
const std::string DEGREE_BASE_PREP_DAILY_STOP_PROB_GTE = "degree.base.prep.daily.stop.prob.gte";

const std::string DEGREE_INTRV_PREP_DAILY_STOP_PROB_LT = "degree.intrv.prep.daily.stop.prob.lt";
const std::string DEGREE_INTRV_PREP_DAILY_STOP_PROB_GTE = "degree.intrv.prep.daily.stop.prob.gte";
const std::string DEGREE_PREP_YEARLY_INCREMENT_LT = "degree.intrv.prep.yearly.increment.lt";
const std::string DEGREE_PREP_YEARLY_INCREMENT_GTE = "degree.intrv.prep.yearly.increment.gte";
const std::string DEGREE_PREP_YEARS_TO_INCREMENT = "degree.intrv.prep.years.to.increment";
const std::string DEGREE_TOPN = "degree.intrv.prep.topn";

const std::string RANDOM_BASE_PREP_USE_PROP_LT = "random.base.prep.bl.use.prop.lt";
const std::string RANDOM_BASE_PREP_USE_PROP_GTE = "random.base.prep.bl.use.prop.gte";
const std::string RANDOM_BASE_PREP_DAILY_STOP_PROB_LT = "random.base.prep.daily.stop.prob.lt";
const std::string RANDOM_BASE_PREP_DAILY_STOP_PROB_GTE = "random.base.prep.daily.stop.prob.gte";

const std::string RANDOM_INTRV_PREP_DAILY_STOP_PROB_LT = "random.intrv.prep.daily.stop.prob.lt";
const std::string RANDOM_INTRV_PREP_DAILY_STOP_PROB_GTE = "random.intrv.prep.daily.stop.prob.gte";
const std::string RANDOM_PREP_YEARLY_INCREMENT_LT = "random.intrv.prep.yearly.increment.lt";
const std::string RANDOM_PREP_YEARLY_INCREMENT_GTE = "random.intrv.prep.yearly.increment.gte";
const std::string RANDOM_PREP_YEARS_TO_INCREMENT = "random.intrv.prep.years.to.increment";

const std::string SET_POINT_VIRAL_LOAD = "set.point.viral.load";
const std::string SIZE_OF_TIMESTEP = "size.of.timestep";
const std::string TIME_INFECTION_TO_LATE_STAGE = "time.infection.to.late.stage";
const std::string TIME_INFECTION_TO_PEAK_VIRAL_LOAD = "time.infection.to.peak.viral.load";
const std::string TIME_INFECTION_TO_PEAK_VIREMIA = "time.infection.to.peak.viremia";
const std::string TIME_INFECTION_TO_VIRAL_SET_POINT = "time.infection.to.viral.set.point";
const std::string TIME_TO_FULL_SUPP = "time.to.full.supp";
const std::string UNDETECTABLE_VL = "undetectable.vl";
const std::string UNINFECTED_CD4_LEVEL = "uninfected.cd4.level";
const std::string UNTREATED_CD4_DAILY_DECLINE = "untreated.cd4.daily.decline";
const std::string UNTREATED_CD4_PERSTEP_DECLINE = "untreated.cd4.perstep.decline";
const std::string VIRAL_LOAD_LOG_INCREMENT = "inf.inc.log.vl";

const std::string PR_INSERTIVE_MAIN = "pr_insertive_main";
const std::string PR_RECEPTIVE_MAIN = "pr_receptive_main";

const std::string PR_INSERTIVE_CASUAL = "pr_insertive_casual";
const std::string PR_RECEPTIVE_CASUAL = "pr_receptive_casual";

const std::string PROP_STEADY_SEX_ACTS = "prop.steady.sex.acts";
const std::string PROP_CASUAL_SEX_ACTS = "prop.casual.sex.acts";
const std::string INFECTIVITY_REDUCTION_CONDOM = "inf.red.w.condom";

const std::string ART_LAG_PREFIX_LT = "art.init.lag.lt";
const std::string ART_LAG_PREFIX_GTE = "art.init.lag.gte";
const std::string ASM_PREFIX = "asm.";
const std::string CD4M_TREATED_PREFIX = "cd4m_treated.";

const std::string TESTING_PROB_PREFIX_LT = "testing.prob.lt";
const std::string TESTING_PROB_PREFIX_GTE = "testing.prob.gte";

const std::string PARTIAL_ART_ADHER_WINDOW_LENGTH = "partial.art_adher.window.length";
const std::string ART_PROP_NEVER_ADHERENT = "art.prop.never.adherent";
const std::string ART_PROP_ALWAYS_ADHERENT = "art.prop.always.adherent";
const std::string ART_PROP_PARTIAL_POS_ADHERENT = "art.prop.part.plus.adherent";
const std::string ART_PROP_PARTIAL_NEG_ADHERENT = "art.prop.part.neg.adherent";

const std::string ART_ALWAYS_ADHERENT_PROB = "art.always.adherent.probability";
const std::string ART_NEVER_ADHERENT_PROB = "art.never.adherent.probability";
const std::string ART_PARTIAL_POS_ADHERENT_PROB = "art.partial.pos.adherent.probability";
const std::string ART_PARTIAL_NEG_ADHERENT_PROB = "art.partial.neg.adherent.probability";

const std::string PREP_PROP_NEVER_ADHERENT = "prep.prop.never.adherent";
const std::string PREP_PROP_ALWAYS_ADHERENT = "prep.prop.always.adherent";
const std::string PREP_PROP_PARTIAL_POS_ADHERENT = "prep.prop.part.plus.adherent";
const std::string PREP_PROP_PARTIAL_NEG_ADHERENT = "prep.prop.part.neg.adherent";

const std::string PREP_ALWAYS_ADHERENT_TR = "prep.always.adherent.trans.reduction";
const std::string PREP_NEVER_ADHERENT_TR = "prep.never.adherent.trans.reduction";
const std::string PREP_PARTIAL_POS_ADHERENT_TR = "prep.partial.pos.adherent.trans.reduction";
const std::string PREP_PARTIAL_NEG_ADHERENT_TR = "prep.partial.neg.adherent.trans.reduction";

const std::string SD_STEADY_NEVER_USE_CONDOMS = "sd.steady.never.use.condoms";
const std::string SD_STEADY_RARELY_USE_CONDOMS = "sd.steady.rarely.use.condoms";
const std::string SD_STEADY_SOMETIMES_USE_CONDOMS ="sd.steady.sometimes.use.condoms";
const std::string SD_STEADY_USUALLY_USE_CONDOMS = "sd.steady.usually.use.condoms";
const std::string SD_STEADY_ALWAYS_USE_CONDOMS = "sd.steady.always.use.condoms";

const std::string SD_CASUAL_NEVER_USE_CONDOMS = "sd.casual.never.use.condoms";
const std::string SD_CASUAL_RARELY_USE_CONDOMS = "sd.casual.rarely.use.condoms";
const std::string SD_CASUAL_SOMETIMES_USE_CONDOMS = "sd.casual.sometimes.use.condoms";
const std::string SD_CASUAL_USUALLY_USE_CONDOMS = "sd.casual.usually.use.condoms";
const std::string SD_CASUAL_ALWAYS_USE_CONDOMS = "sd.casual.always.use.condoms";

const std::string SC_STEADY_NEVER_USE_CONDOMS = "sc.steady.never.use.condoms";
const std::string SC_STEADY_RARELY_USE_CONDOMS = "sc.steady.rarely.use.condoms";
const std::string SC_STEADY_SOMETIMES_USE_CONDOMS ="sc.steady.sometimes.use.condoms";
const std::string SC_STEADY_USUALLY_USE_CONDOMS = "sc.steady.usually.use.condoms";
const std::string SC_STEADY_ALWAYS_USE_CONDOMS = "sc.steady.always.use.condoms";

const std::string SC_CASUAL_NEVER_USE_CONDOMS = "sc.casual.never.use.condoms";
const std::string SC_CASUAL_RARELY_USE_CONDOMS = "sc.casual.rarely.use.condoms";
const std::string SC_CASUAL_SOMETIMES_USE_CONDOMS = "sc.casual.sometimes.use.condoms";
const std::string SC_CASUAL_USUALLY_USE_CONDOMS = "sc.casual.usually.use.condoms";
const std::string SC_CASUAL_ALWAYS_USE_CONDOMS = "sc.casual.always.use.condoms";

const std::string SD_STEADY_NEVER_USE_CONDOMS_PROB = "sd.steady.never.use.condoms.prob";
const std::string SD_STEADY_RARELY_USE_CONDOMS_PROB = "sd.steady.rarely.use.condoms.prob";
const std::string SD_STEADY_SOMETIMES_USE_CONDOMS_PROB ="sd.steady.sometimes.use.condoms.prob";
const std::string SD_STEADY_USUALLY_USE_CONDOMS_PROB = "sd.steady.usually.use.condoms.prob";
const std::string SD_STEADY_ALWAYS_USE_CONDOMS_PROB = "sd.steady.always.use.condoms.prob";

const std::string SD_CASUAL_NEVER_USE_CONDOMS_PROB = "sd.casual.never.use.condoms.prob";
const std::string SD_CASUAL_RARELY_USE_CONDOMS_PROB = "sd.casual.rarely.use.condoms.prob";
const std::string SD_CASUAL_SOMETIMES_USE_CONDOMS_PROB = "sd.casual.sometimes.use.condoms.prob";
const std::string SD_CASUAL_USUALLY_USE_CONDOMS_PROB = "sd.casual.usually.use.condoms.prob";
const std::string SD_CASUAL_ALWAYS_USE_CONDOMS_PROB = "sd.casual.always.use.condoms.prob";

const std::string SC_STEADY_NEVER_USE_CONDOMS_PROB = "sc.steady.never.use.condoms.prob";
const std::string SC_STEADY_RARELY_USE_CONDOMS_PROB = "sc.steady.rarely.use.condoms.prob";
const std::string SC_STEADY_SOMETIMES_USE_CONDOMS_PROB ="sc.steady.sometimes.use.condoms.prob";
const std::string SC_STEADY_USUALLY_USE_CONDOMS_PROB = "sc.steady.usually.use.condoms.prob";
const std::string SC_STEADY_ALWAYS_USE_CONDOMS_PROB = "sc.steady.always.use.condoms.prob";

const std::string SC_CASUAL_NEVER_USE_CONDOMS_PROB = "sc.casual.never.use.condoms.prob";
const std::string SC_CASUAL_RARELY_USE_CONDOMS_PROB = "sc.casual.rarely.use.condoms.prob";
const std::string SC_CASUAL_SOMETIMES_USE_CONDOMS_PROB = "sc.casual.sometimes.use.condoms.prob";
const std::string SC_CASUAL_USUALLY_USE_CONDOMS_PROB = "sc.casual.usually.use.condoms.prob";
const std::string SC_CASUAL_ALWAYS_USE_CONDOMS_PROB = "sc.casual.always.use.condoms.prob";

const std::string EXTERNAL_INFECTION_RATE_MIN = "external.infections.per.person.day.min";
const std::string EXTERNAL_INFECTION_RATE_MAX = "external.infections.per.person.day.max";
const std::string EXTERNAL_INFECTION_AGE_FACTOR = "external.infections.age.factor";

// jail related:
//const std::string INPUT_LOWER_JAIL_TERM_PROB = "min.jail.term.duration";
//const std::string INPUT_UPPER_JAIL_TERM_PROB = "max.jail.term.duration";

const std::string NETWORK_RETENTION_MULTIPLIER = "network.retention.multiplier";
const std::string NETWORK_DECAY_PROB_MAIN_FILE   = "network.decay.prob.main.file";
const std::string NETWORK_DECAY_PROB_CASUAL_FILE = "network.decay.prob.casual.file";

const std::string IS_NETWORK_DISRUPTION_ON = "is.network.disruption.on";
const std::string IS_CARE_DISRUPTION_ON = "is.care.disruption.on";

const std::string INCARCERATION_PROB = "incarceration.prob";
// prob for previously incarcerated
const std::string INCARCERATION_WITH_CJI_PROB = "incarceration.with.cji.prob";

// jail serving time (term) mean (in terms of days)
const std::string JAIL_SERVING_TIME_MEAN = "jail.serving.time.mean";

const std::string JAIL_INFECTION_RATE_WINDOW_SIZE = "jail.infection.rate.window.size";
const std::string JAIL_INFECTION_RATE_MULTIPLIER = "jail.infection.rate.multiplier";
const std::string JAIL_INFECTION_RATE_DEFAULT = "jail.infection.rate.default";

const std::string POST_RELEASE_INTERFERENCE_PERIOD_MEAN = "post.release.interference.period.mean";

const std::string JAIL_INFECTION_RATE_MULTIPLIER_UPDATE = "jail.infection.rate.multiplier.update";
const std::string INCARCERATION_PROB_UPDATE = "incarceration.prob.update";
const std::string INCARCERATION_WITH_CJI_PROB_UPDATE = "incarceration.with.cji.prob.update";
const std::string JAIL_SERVING_TIME_MEAN_UPDATE = "jail.serving.time.mean.update";
const std::string NETWORK_RETENTION_MULTIPLIER_UPDATE = "network.retention.multiplier.update";
const std::string POST_RELEASE_INTERFERENCE_PERIOD_MEAN_UPDATE = "post.release.interference.period.mean.update";
const std::string IS_NETWORK_DISRUPTION_ON_UPDATE = "is.network.disruption.on.update";
const std::string IS_CARE_DISRUPTION_ON_UPDATE = "is.care.disruption.on.update";

const std::string METH_PROP = "meth.prop";
const std::string CRACK_PROP = "crack.prop";
const std::string ECSTASY_PROP = "ecstasy.prop";

const std::string STIMULANT_USE_MORTALITY_INCREASE = "stimulant.use.mortality.increase";

const std::string MORTALITY_RATE_METH = "mortality.rate.meth";
const std::string MORTALITY_RATE_CRACK = "mortality.rate.crack";

const std::string SUBSTANCE_USE_INTERVENTION = "substance.use.intervention";

Parameters* Parameters::instance_ = 0;

std::ostream& operator<< (std::ostream& os, const Parameters* params) {
    for (auto iter = params->props_.keys_begin(); iter != params->props_.keys_end(); ++iter) {
        os << (*iter) << " : " << params->getStringParameter(*iter) << std::endl;
    }
    return os;
}

Parameters::Parameters(repast::Properties& props) :
        props_(props)  {
}

Parameters::~Parameters() {

}

void Parameters::initialize(Properties& props) {
    if (instance_ != 0) {
        delete instance_;
    }
    instance_ = new Parameters(props);
}

Parameters* Parameters::instance() {
    if (instance_ == 0)
        throw std::domain_error("Parameters must be initialized before instance() is called");

    return instance_;
}

std::string Parameters::getStringParameter(const std::string& prop_name) const {
    std::string val = props_.getProperty(prop_name);
    if (val.length() == 0)
        throw std::invalid_argument(
                "Invalid property name '" + prop_name + "', no property found.");
    return val;
}

bool Parameters::contains(const std::string& prop_name) const {
    return props_.contains(prop_name);
}

int Parameters::getIntParameter(const std::string& prop_name) const {
    return strToInt(getStringParameter(prop_name));
}

float Parameters::getFloatParameter(const std::string& prop_name) const {
    return std::stof(getStringParameter(prop_name));
}

double Parameters::getDoubleParameter(const std::string& prop_name) const {
    return std::stod(getStringParameter(prop_name));
}

void Parameters::getKeys(const std::string& starts_with, std::vector<std::string>& keys) {
    for (auto iter = props_.keys_begin(); iter != props_.keys_end(); ++iter) {
        // std::cout << (*iter) << std::endl;
        if (boost::starts_with(*iter, starts_with)) {
            keys.push_back(*iter);
        }
    }
}

void Parameters::putParameter(const std::string& key, bool value) {
    std::string val = value ? "true" : "false";
    props_.putProperty(key, val);
}

void Parameters::putParameter(const std::string& key, const std::string& value) {
    props_.putProperty(key, value);
}

void Parameters::putParameter(const std::string& key, double value) {
    props_.putProperty(key, value);
}

bool Parameters::getBooleanParameter(const std::string& prop_name) const {
    std::string val = getStringParameter(prop_name);
    return val == "true" || val == "TRUE";
}

} /* namespace mrsa */
