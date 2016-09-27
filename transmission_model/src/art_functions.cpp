/*
 * art_functions.cpp
 *
 *  Created on: Sep 26, 2016
 *      Author: nick
 */

#include "repast_hpc/RepastProcess.h"

#include "art_functions.h"
#include "AdherenceCategorySelector.h"
#include "AdherenceCheckScheduler.h"
#include "AdherenceCategory.h"
#include "Parameters.h"

namespace TransModel {

void initialize_adherence(std::shared_ptr<Person> person, double first_art_at_tick) {
	double always = Parameters::instance()->getDoubleParameter(PROP_ALWAYS_ADHERENT);
	double never = Parameters::instance()->getDoubleParameter(PROP_NEVER_ADHERENT);
	double partial = Parameters::instance()->getDoubleParameter(PROP_PARTIALLY_ADHERENT);
	AdherenceCategorySelector selector(never, always, partial);
	person->setAdherence(selector.selectCategory());
	if (person->adherence() == AdherenceCategory::PARTIAL) {
		schedule_adherence(person, first_art_at_tick);
	}
}

void schedule_adherence(std::shared_ptr<Person> person, double first_art_at_tick) {
	double adherence_check_at = first_art_at_tick
			+ Parameters::instance()->getDoubleParameter(PARTIAL_ART_ADHER_WINDOW_LENGTH);
	AdherenceCheckScheduler* ac_scheduler = new AdherenceCheckScheduler(person, adherence_check_at);
	repast::RepastProcess::instance()->getScheduleRunner().scheduleEvent(adherence_check_at,
			repast::Schedule::FunctorPtr(ac_scheduler));

}

}

