/*
 * Parameters.cpp
 *
 *  Created on: Jun 5, 2012
 *      Author: nick
 */

#include "Parameters.h"
#include "repast_hpc/Utilities.h"

namespace TransModel {

using namespace repast;

const std::string R_FILE = "r.file";
const std::string MAX_SURVIVAL = "max.survival";
const std::string DAILY_DEATH_PROB = "daily.death.prob";
const std::string DAILY_BIRTH_RATE = "daily.birth.rate";

const std::string CIRC_MULT = "circumcision.multiplier";
const std::string PREP_MULT = "prep.multiplier";
const std::string DUR_INF_BY_AGE = "duration.of.infection.by.age";

const std::string SIZE_OF_TIMESTEP = "size.of.timestep";

const std::string ART_COVERAGE_RATE = "art.coverage.rate";

const std::string CD4_RECOVERY_TIME = "cd4.recovery.time";
const std::string CD4_AT_INFECTION = "cd4.at.infection";
const std::string PER_DAY_CD4_RECOVERY = "per.day.cd4.recovery";

const std::string TIME_INFECTION_TO_PEAK_LOAD = "time.infection.to.peak.load";
const std::string TIME_INFECTION_TO_SET_POINT = "time.infection.to.set.point";
const std::string TIME_INFECTION_TO_LATE_STAGE = "time.infection.to.late.stage";
const std::string TIME_TO_FULL_SUPP = "time.to.full.supp";
const std::string PEAK_VIRAL_LOAD = "peak.viral.load";
const std::string SET_POINT_VIRAL_LOAD = "set.point.viral.load";
const std::string LATE_STAGE_VIRAL_LOAD = "late.stage.viral.load";
const std::string UNDETECTABLE_VIRAL_LOAD = "undetectable.viral.load";

const std::string ACUTE_RANGE_MAX_NUMERATOR = "acute.range.max.numerator";
const std::string CHRONIC_RANGE_MAX_NUMERATOR = "chronic.range.max.numerator";
const std::string LATE_RANGE_MAX_NUMERATOR = "late.range.max.numerator";
const std::string MIN_CHRONIC_INFECTIVITY_UNADJ = "min.chronic.infectivity.unadj";
const std::string ACUTE_MULTIPLIER = "acute.multiplier";
const std::string CHRONIC_MULTIPLIER = "chronic.multiplier";
const std::string LATE_MULTIPLIER = "late_multiplier";

const std::string SEX_ACTS_PER_TIMESTEP = "sex.acts.per.timestep";


Parameters* Parameters::instance_ = 0;

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

int Parameters::getIntParameter(const std::string& prop_name) const {
	return strToInt(getStringParameter(prop_name));
}

double Parameters::getDoubleParameter(const std::string& prop_name) const {
	return strToDouble(getStringParameter(prop_name));
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
