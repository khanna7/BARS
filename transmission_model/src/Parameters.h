#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include "repast_hpc/Properties.h"

namespace TransModel {

extern const std::string RUN_NUMBER;

extern const std::string INPUT_AGE_THRESHOLD;

extern const std::string LT_SUFFIX;
extern const std::string GTE_SUFFIX;

extern const std::string NET_VAR;
extern const std::string CASUAL_NET_VAR;

extern const std::string STOP_AT;
extern const std::string R_PARAMETERS_DERIVED;
extern const std::string R_PARAMETERS_NON_DERIVED;
extern const std::string R_FILE;
extern const std::string INIT_DATA_FILE;
extern const std::string MAIN_NETWORK_FILE;
extern const std::string CASUAL_NETWORK_FILE;
extern const std::string OUTPUT_DIR;
extern const std::string COUNTS_PER_TIMESTEP_OUTPUT_FILE;
extern const std::string PARTNERSHIP_EVENTS_FILE;
extern const std::string INFECTION_EVENTS_FILE;
extern const std::string BIOMARKER_FILE;
extern const std::string BIOMARKER_LOG_COUNT;
extern const std::string DEATH_EVENT_FILE;
extern const std::string TESTING_EVENT_FILE;
extern const std::string ART_EVENT_FILE;
extern const std::string PREP_EVENT_FILE;
extern const std::string PERSON_DATA_FILE;
extern const std::string NET_SAVE_FILE;
extern const std::string CASUAL_NET_SAVE_FILE;
extern const std::string NET_SAVE_AT;
extern const std::string COUNT_OVERLAPS;

extern const std::string SERO_LOG_FILE;
extern const std::string BASE_LOG_FILE;
extern const std::string NET_LOG_FILE;
extern const std::string RANDOM_SELECTION_LOG_FILE;

extern const std::string EVENT_FILE;
extern const std::string EVENT_FILE_BUFFER_SIZE;

// generated from parameters.R
extern const std::string ACUTE_LENGTH_MIN;
extern const std::string ACUTE_LENGTH_MAX;
extern const std::string ACUTE_MULT;
extern const std::string ACUTE_MULT_HOLLING;
extern const std::string B1_REF;
extern const std::string B2_AFRICAN;
extern const std::string B3_FEMALE;
extern const std::string B4_CD4_REF;
extern const std::string B5_AFRICAN;
extern const std::string B6_AGE_15TO29;
extern const std::string B6_AGE_30TO39;
extern const std::string B6_AGE_40TO49;
extern const std::string B6_AGE_50ORMORE;
extern const std::string CD4_AT_INFECTION_MALE;
extern const std::string CD4_REC_PER_TIMESTEP;
extern const std::string CD4_RECOVERY_TIME;
extern const std::string CHRONIC_LENGTH_MIN;
extern const std::string CHRONIC_LENGTH_MAX;
extern const std::string CIRCUM_MULT;
extern const std::string CIRCUM_RATE;
extern const std::string DAILY_ENTRY_RATE;
extern const std::string DETECTION_WINDOW;
extern const std::string DUR_INF;
extern const std::string DURATION_OF_INFECTION;
extern const std::string INIT_HIV_PREV;
extern const std::string INIT_HIV_PREV_ENTRIES;
extern const std::string INFECTIVE_INSERTIVE_MULT;
extern const std::string LATE_LENGTH_MIN;
extern const std::string LATE_LENGTH_MAX;
extern const std::string LATE_MULT;
extern const std::string LATE_MULT_HOLLING;
extern const std::string LATE_STAGE_VIRAL_LOAD;
extern const std::string MAX_AGE;
extern const std::string MIN_AGE;
extern const std::string MIN_CHRONIC_INFECTIVITY_UNADJ;
extern const std::string NON_TESTERS_PROP_LT;
extern const std::string NON_TESTERS_PROP_GTE;
extern const std::string NUM_SEX_ACTS_PER_TIMESTEP;
extern const std::string PEAK_VIRAL_LOAD;
extern const std::string PER_DAY_CD4_RECOVERY;
extern const std::string PREG_MULT;
extern const std::string PREG_SUSC_MULT;

extern const std::string PREP_SCHEME;

extern const std::string DEFAULT_PREP_USE_PROP_LT;
extern const std::string DEFAULT_PREP_USE_PROP_GTE;
extern const std::string DEFAULT_PREP_DAILY_STOP_PROB_LT;
extern const std::string DEFAULT_PREP_DAILY_STOP_PROB_GTE;
extern const std::string DEFAULT_PREP_YEARLY_INCREMENT_LT;
extern const std::string DEFAULT_PREP_YEARLY_INCREMENT_GTE;
extern const std::string DEFAULT_PREP_YEARS_TO_INCREMENT;
extern const std::string DEFAULT_PREP_BALANCED_UNBALANCED;
extern const std::string DEFAULT_PREP_UNBALANCED_STARTING_PROB_LT;
extern const std::string DEFAULT_PREP_UNBALANCED_STARTING_PROB_GTE;

extern const std::string YOR_ADDITIONAL_PREP_LT;
extern const std::string YOR_ADDITIONAL_PREP_GTE;
extern const std::string YOR_PREP_ALPHA;
extern const std::string YOR_PREP_USE_PROP;
extern const std::string YOR_PREP_DAILY_STOP_PROB;
extern const std::string YOR_PREP_YEARLY_INCREMENT;
extern const std::string YOR_PREP_YEARS_TO_INCREMENT;

extern const std::string SERO_BASE_PREP_USE_PROP_LT;
extern const std::string SERO_BASE_PREP_USE_PROP_GTE;
extern const std::string SERO_BASE_PREP_DAILY_STOP_PROB_LT;
extern const std::string SERO_BASE_PREP_DAILY_STOP_PROB_GTE;

extern const std::string SERO_INTRV_PREP_DAILY_STOP_PROB_LT;
extern const std::string SERO_INTRV_PREP_DAILY_STOP_PROB_GTE;
extern const std::string SERO_PREP_YEARLY_INCREMENT_LT;
extern const std::string SERO_PREP_YEARLY_INCREMENT_GTE;
extern const std::string SERO_PREP_YEARS_TO_INCREMENT;
extern const std::string SERO_NET_TYPE;

extern const std::string EIGEN_BASE_PREP_USE_PROP_LT;
extern const std::string EIGEN_BASE_PREP_USE_PROP_GTE;
extern const std::string EIGEN_BASE_PREP_DAILY_STOP_PROB_LT;
extern const std::string EIGEN_BASE_PREP_DAILY_STOP_PROB_GTE;


extern const std::string EIGEN_INTRV_PREP_DAILY_STOP_PROB_LT;
extern const std::string EIGEN_INTRV_PREP_DAILY_STOP_PROB_GTE;
extern const std::string EIGEN_PREP_YEARLY_INCREMENT_LT;
extern const std::string EIGEN_PREP_YEARLY_INCREMENT_GTE;
extern const std::string EIGEN_PREP_YEARS_TO_INCREMENT;
extern const std::string EIGEN_TOPN;

extern const std::string DEGREE_BASE_PREP_USE_PROP_LT;
extern const std::string DEGREE_BASE_PREP_USE_PROP_GTE;
extern const std::string DEGREE_BASE_PREP_DAILY_STOP_PROB_LT;
extern const std::string DEGREE_BASE_PREP_DAILY_STOP_PROB_GTE;

extern const std::string DEGREE_INTRV_PREP_DAILY_STOP_PROB_LT;
extern const std::string DEGREE_INTRV_PREP_DAILY_STOP_PROB_GTE;
extern const std::string DEGREE_PREP_YEARLY_INCREMENT_LT;
extern const std::string DEGREE_PREP_YEARLY_INCREMENT_GTE;
extern const std::string DEGREE_PREP_YEARS_TO_INCREMENT;
extern const std::string DEGREE_TOPN;

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
extern const std::string VIRAL_LOAD_LOG_INCREMENT;

extern const std::string PR_INSERTIVE_MAIN;
extern const std::string PR_RECEPTIVE_MAIN;

extern const std::string PR_INSERTIVE_CASUAL;
extern const std::string PR_RECEPTIVE_CASUAL;

extern const std::string PROP_STEADY_SEX_ACTS;
extern const std::string PROP_CASUAL_SEX_ACTS;
extern const std::string INFECTIVITY_REDUCTION_CONDOM;

extern const std::string ART_LAG_PREFIX_LT;
extern const std::string ART_LAG_PREFIX_GTE;
extern const std::string ASM_PREFIX;
extern const std::string CD4M_TREATED_PREFIX;
extern const std::string TESTING_PROB_PREFIX_LT;
extern const std::string TESTING_PROB_PREFIX_GTE;

extern const std::string PARTIAL_ART_ADHER_WINDOW_LENGTH;
extern const std::string ART_PROP_NEVER_ADHERENT;
extern const std::string ART_PROP_ALWAYS_ADHERENT;
extern const std::string ART_PROP_PARTIAL_POS_ADHERENT;
extern const std::string ART_PROP_PARTIAL_NEG_ADHERENT;

extern const std::string ART_ALWAYS_ADHERENT_PROB;
extern const std::string ART_NEVER_ADHERENT_PROB;
extern const std::string ART_PARTIAL_POS_ADHERENT_PROB;
extern const std::string ART_PARTIAL_NEG_ADHERENT_PROB;

extern const std::string PREP_PROP_NEVER_ADHERENT;
extern const std::string PREP_PROP_ALWAYS_ADHERENT;
extern const std::string PREP_PROP_PARTIAL_POS_ADHERENT;
extern const std::string PREP_PROP_PARTIAL_NEG_ADHERENT;

extern const std::string PREP_ALWAYS_ADHERENT_TR;
extern const std::string PREP_NEVER_ADHERENT_TR;
extern const std::string PREP_PARTIAL_POS_ADHERENT_TR;
extern const std::string PREP_PARTIAL_NEG_ADHERENT_TR;

extern const std::string SD_STEADY_NEVER_USE_CONDOMS;
extern const std::string SD_STEADY_RARELY_USE_CONDOMS;
extern const std::string SD_STEADY_SOMETIMES_USE_CONDOMS;
extern const std::string SD_STEADY_USUALLY_USE_CONDOMS;
extern const std::string SD_STEADY_ALWAYS_USE_CONDOMS;

extern const std::string SD_CASUAL_NEVER_USE_CONDOMS;
extern const std::string SD_CASUAL_RARELY_USE_CONDOMS;
extern const std::string SD_CASUAL_SOMETIMES_USE_CONDOMS;
extern const std::string SD_CASUAL_USUALLY_USE_CONDOMS;
extern const std::string SD_CASUAL_ALWAYS_USE_CONDOMS;

extern const std::string SC_STEADY_NEVER_USE_CONDOMS;
extern const std::string SC_STEADY_RARELY_USE_CONDOMS;
extern const std::string SC_STEADY_SOMETIMES_USE_CONDOMS;
extern const std::string SC_STEADY_USUALLY_USE_CONDOMS;
extern const std::string SC_STEADY_ALWAYS_USE_CONDOMS;

extern const std::string SC_CASUAL_NEVER_USE_CONDOMS;
extern const std::string SC_CASUAL_RARELY_USE_CONDOMS;
extern const std::string SC_CASUAL_SOMETIMES_USE_CONDOMS;
extern const std::string SC_CASUAL_USUALLY_USE_CONDOMS;
extern const std::string SC_CASUAL_ALWAYS_USE_CONDOMS;

extern const std::string SD_STEADY_NEVER_USE_CONDOMS_PROB;
extern const std::string SD_STEADY_RARELY_USE_CONDOMS_PROB;
extern const std::string SD_STEADY_SOMETIMES_USE_CONDOMS_PROB;
extern const std::string SD_STEADY_USUALLY_USE_CONDOMS_PROB;
extern const std::string SD_STEADY_ALWAYS_USE_CONDOMS_PROB;

extern const std::string SD_CASUAL_NEVER_USE_CONDOMS_PROB;
extern const std::string SD_CASUAL_RARELY_USE_CONDOMS_PROB;
extern const std::string SD_CASUAL_SOMETIMES_USE_CONDOMS_PROB;
extern const std::string SD_CASUAL_USUALLY_USE_CONDOMS_PROB;
extern const std::string SD_CASUAL_ALWAYS_USE_CONDOMS_PROB;

extern const std::string SC_STEADY_NEVER_USE_CONDOMS_PROB;
extern const std::string SC_STEADY_RARELY_USE_CONDOMS_PROB;
extern const std::string SC_STEADY_SOMETIMES_USE_CONDOMS_PROB;
extern const std::string SC_STEADY_USUALLY_USE_CONDOMS_PROB;
extern const std::string SC_STEADY_ALWAYS_USE_CONDOMS_PROB;

extern const std::string SC_CASUAL_NEVER_USE_CONDOMS_PROB;
extern const std::string SC_CASUAL_RARELY_USE_CONDOMS_PROB;
extern const std::string SC_CASUAL_SOMETIMES_USE_CONDOMS_PROB;
extern const std::string SC_CASUAL_USUALLY_USE_CONDOMS_PROB;
extern const std::string SC_CASUAL_ALWAYS_USE_CONDOMS_PROB;

extern const std::string EXTERNAL_INFECTION_RATE_MIN;
extern const std::string EXTERNAL_INFECTION_RATE_MAX;
extern const std::string EXTERNAL_INFECTION_AGE_FACTOR;

extern const std::string INPUT_LOWER_JAIL_TERM_PROB;
extern const std::string INPUT_UPPER_JAIL_TERM_PROB;
extern const std::string INCARCERATION_PROB_FOR_ENTRIES;

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
	 * Gets all the parmaeter keys that start with the specified prefix.
	 *
	 */
	void getKeys(const std::string& starts_with, std::vector<std::string>& keys);

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

	bool contains(const std::string& prop_name) const;

private:
	friend std::ostream& operator<< (std::ostream& stream, const Parameters* params);

	static Parameters* instance_;
	repast::Properties props_;

	Parameters(repast::Properties& props);
};

std::ostream& operator<< (std::ostream& stream, const Parameters* params);

} /* namespace mrsa */
#endif /* PARAMETERS_H_ */
