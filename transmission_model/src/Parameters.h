#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include "repast_hpc/Properties.h"

namespace TransModel {

extern const std::string STOP_AT;
extern const std::string R_PARAMETERS_FILE;
extern const std::string R_FILE;
extern const std::string COUNTS_PER_TIMESTEP_OUTPUT_FILE;
extern const std::string PARTNERSHIP_EVENTS_FILE;
extern const std::string INFECTION_EVENTS_FILE;

extern const std::string EVENT_FILE;
extern const std::string EVENT_FILE_BUFFER_SIZE;

extern const std::string PREP_MULT;

// generated from parameters.R
extern const std::string ACUTE_LENGTH_MIN;
extern const std::string ACUTE_LENGTH_MAX;
extern const std::string ACUTE_MULT;
extern const std::string ACUTE_MULT_HOLLING;
extern const std::string ART_INIT_TIME;
extern const std::string B1_REF;
extern const std::string B2_AFRICAN;
extern const std::string B3_FEMALE;
extern const std::string B4_CD4_REF;
extern const std::string B5_AFRICAN;
extern const std::string B6_AGE_15TO29;
extern const std::string B6_AGE_30TO39;
extern const std::string B6_AGE_40TO49;
extern const std::string B6_AGE_50ORMORE;
extern const std::string BASELINE_ART_COVERAGE_RATE;
extern const std::string CD4_AT_INFECTION_MALE;
extern const std::string CD4_REC_PER_TIMESTEP;
extern const std::string CD4_RECOVERY_TIME;
extern const std::string CHRONIC_LENGTH_MIN;
extern const std::string CHRONIC_LENGTH_MAX;
extern const std::string CIRCUM_MULT;
extern const std::string CIRCUM_RATE;
extern const std::string DAILY_ENTRY_RATE;
extern const std::string DUR_INF;
extern const std::string DURATION;
extern const std::string DURATION_OF_INFECTION;
extern const std::string DUR_INF_BY_AGE;
extern const std::string INIT_HIV_PREV;
extern const std::string LATE_LENGTH_MIN;
extern const std::string LATE_LENGTH_MAX;
extern const std::string LATE_MULT;
extern const std::string LATE_MULT_HOLLING;
extern const std::string LATE_STAGE_VIRAL_LOAD;
extern const std::string MAX_AGE;
extern const std::string MIN_AGE;
extern const std::string MIN_CHRONIC_INFECTIVITY_UNADJ;
extern const std::string NUM_SEX_ACTS_PER_TIMESTEP;
extern const std::string PEAK_VIRAL_LOAD;
extern const std::string PER_DAY_CD4_RECOVERY;
extern const std::string PREG_MULT;
extern const std::string PREG_SUSC_MULT;
extern const std::string SET_POINT_VIRAL_LOAD;
extern const std::string SIZE_OF_TIMESTEP;
extern const std::string TIME_INFECTION_TO_LATE_STAGE;
extern const std::string TIME_INFECTION_TO_PEAK_VIRAL_LOAD;
extern const std::string TIME_INFECTION_TO_PEAK_VIREMIA;
extern const std::string TIME_INFECTION_TO_VIRAL_SET_POINT;
extern const std::string TIME_TO_FULL_SUPP;
extern const std::string UNDETECTABLE_VL;
extern const std::string UNINFECTED_CD4_LEVEL;
extern const std::string UNTREATED_CD4_DAILY_DECLINE;
extern const std::string UNTREATED_CD4_PERSTEP_DECLINE;

/**
 * Parameters singleton that contains the model properties.
 *
 */
class Parameters {
public:

	virtual ~Parameters();

	/**
	 * Initializes the Parameters singleton with the specified properties.
	 */
	static void initialize(repast::Properties& props);

	/**
	 * Gets singleton instance. If it has not been initialized, an exception
	 * is thrown.
	 */
	static Parameters* instance();

	/**
	 * Gets the named parameter as a string.
	 *
	 * @return the named parameter.
	 */
	std::string getStringParameter(const std::string& prop_name) const;

	/**
	 * Gets the named parameter as an int.
	 *
	 * @return the named parameter as an int.
	 */
	int getIntParameter(const std::string& prop_name) const;

	/**
	 * Gets the named parameter as a float.
	 *
	 * @return the named parameter as a float.
	 */
	float getFloatParameter(const std::string& prop_name) const;

	/**
	 * Gets the named parameter as a double.
	 *
	 * @return the named parameter as a double.
	 */
	double getDoubleParameter(const std::string& prop_name) const;

	/**
	 * Puts the named parameter into this Parameters object.
	 *
	 * @param key the parameter's name
	 * @param value the parameter's value.
	 */
	void putParameter(const std::string& key, bool value);

	/**
	 * Puts the named parameter into this Parameters object.
	 *
	 * @param key the parameter's name
	 * @param value the parameter's value.
	 */
	void putParameter(const std::string& key, const std::string& value);

	/**
	 * Puts the named double parameter into this Parameters object.
	 *
	 * @param key the parameter's name
	 * @param value the parameter's value.
	 */
	void putParameter(const std::string& key, double value);

	/**
	 * Gets the named parameter as a boolean.
	 *
	 * @return the named parameter.
	 */
	bool getBooleanParameter(const std::string& prop_name) const;

private:
	static Parameters* instance_;
	repast::Properties props_;

	Parameters(repast::Properties& props);
};

} /* namespace mrsa */
#endif /* PARAMETERS_H_ */
