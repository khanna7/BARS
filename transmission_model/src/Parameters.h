

#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include "repast_hpc/Properties.h"

namespace TransModel {

extern const std::string R_FILE;
extern const std::string MAX_SURVIVAL;
extern const std::string DAILY_DEATH_PROB;
extern const std::string DAILY_BIRTH_RATE;

extern const std::string CIRC_MULT;
extern const std::string PREP_MULT;
extern const std::string DUR_INF_BY_AGE;

extern const std::string SIZE_OF_TIMESTEP;

extern const std::string ART_COVERAGE_RATE;

// cd4 params
extern const std::string CD4_RECOVERY_TIME;
extern const std::string CD4_AT_INFECTION;
extern const std::string PER_DAY_CD4_RECOVERY;

// viral load params
extern const std::string TIME_INFECTION_TO_PEAK_LOAD;
extern const std::string TIME_INFECTION_TO_SET_POINT;
extern const std::string TIME_INFECTION_TO_LATE_STAGE;
extern const std::string TIME_TO_FULL_SUPP;
extern const std::string PEAK_VIRAL_LOAD;
extern const std::string SET_POINT_VIRAL_LOAD;
extern const std::string LATE_STAGE_VIRAL_LOAD;
extern const std::string UNDETECTABLE_VIRAL_LOAD;

// 121, 1877, 3300
extern const std::string ACUTE_RANGE_MAX_NUMERATOR;
extern const std::string CHRONIC_RANGE_MAX_NUMERATOR;
extern const std::string LATE_RANGE_MAX_NUMERATOR;
// 0.000497/2.89
extern const std::string MIN_CHRONIC_INFECTIVITY_UNADJ;
// 4.98, 3.49
extern const std::string ACUTE_MULTIPLIER;
extern const std::string LATE_MULTIPLIER;

extern const std::string SEX_ACTS_PER_TIMESTEP;





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
	 * @return the named parameter.
	 */
	int getIntParameter(const std::string& prop_name) const;

	/**
	 * Gets the named parameter as a double.
	 *
	 * @return the named parameter.
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
