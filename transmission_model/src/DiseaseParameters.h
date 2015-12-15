/*
 * DiseaseParameters.h
 *
 *  Created on: Dec 14, 2015
 *      Author: nick
 */

#ifndef SRC_DISEASEPARAMETERS_H_
#define SRC_DISEASEPARAMETERS_H_

namespace TransModel {

struct SharedViralLoadParameters {

	float time_infection_to_peak_load, time_infection_to_set_point, time_infection_to_late_stage, time_to_full_supp;
	float peak_viral_load, set_point_viral_load, late_stage_viral_load, undetectable_viral_load;


	bool lessEqTimeToPeak(float time_since_infection);
	bool greaterTimeToLateStage(float time_since_infection);
	bool withinPeakToSetPoint(float time_since_infection);
	bool withinSetPointToLateStage(float time_since_infection);
};

struct InfectionParameters {

	bool infection_status, art_status;
	float time_since_infection, time_since_art_init, time_since_art_cessation;
	float dur_inf_by_age;
};

struct IndividualViralLoadParameters {

	float viral_load, vl_art_traj_slope;

};


} /* namespace TransModel */

#endif /* SRC_DISEASEPARAMETERS_H_ */
