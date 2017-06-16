/*
 * art_functions.cpp
 *
 *  Created on: Sep 26, 2016
 *      Author: nick
 */


#include "repast_hpc/RepastProcess.h"

#include "adherence_functions.h"
#include "AdherenceCategory.h"
#include "AdherenceCheckScheduler.h"
#include "Parameters.h"
#include "ProbDist.h"

namespace TransModel {

void initialize_art_adherence(std::shared_ptr<Person> person, double tick, AdherenceCategory category) {
	double prob = 0;
	if (category == AdherenceCategory::ALWAYS) {
		prob = Parameters::instance()->getDoubleParameter(ART_ALWAYS_ADHERENT_PROB);
	} else if (category == AdherenceCategory::NEVER) {
		prob = Parameters::instance()->getDoubleParameter(ART_NEVER_ADHERENT_PROB);
	} else if (category == AdherenceCategory::PARTIAL_PLUS) {
		prob = Parameters::instance()->getDoubleParameter(ART_PARTIAL_POS_ADHERENT_PROB);
	}  else if (category == AdherenceCategory::PARTIAL_MINUS) {
		prob = Parameters::instance()->getDoubleParameter(ART_PARTIAL_NEG_ADHERENT_PROB);
	}

	person->setArtAdherence({prob, category});
	schedule_art_adherence(person, tick);
}

void initialize_art_adherence(std::shared_ptr<Person> person, double first_art_at_tick) {
	double always = Parameters::instance()->getDoubleParameter(ART_PROP_ALWAYS_ADHERENT);
	double never = Parameters::instance()->getDoubleParameter(ART_PROP_NEVER_ADHERENT);
	double partial_plus = Parameters::instance()->getDoubleParameter(ART_PROP_PARTIAL_POS_ADHERENT);
	double partial_minus = Parameters::instance()->getDoubleParameter(ART_PROP_PARTIAL_NEG_ADHERENT);

	//std::cout << "art: " << always << "," << never << "," << partial_plus << ", " << partial_minus << std::endl;

	double always_prob = Parameters::instance()->getDoubleParameter(ART_ALWAYS_ADHERENT_PROB);
	double never_prob = Parameters::instance()->getDoubleParameter(ART_NEVER_ADHERENT_PROB);
	double partial_plus_prob = Parameters::instance()->getDoubleParameter(ART_PARTIAL_POS_ADHERENT_PROB);
	double partial_minus_prob = Parameters::instance()->getDoubleParameter(ART_PARTIAL_NEG_ADHERENT_PROB);

	ProbDistCreator<AdherenceData> creator;
	creator.addItem(always, std::make_shared<AdherenceData>(always_prob, AdherenceCategory::ALWAYS));
	creator.addItem(never, std::make_shared<AdherenceData>(never_prob, AdherenceCategory::NEVER));
	creator.addItem(partial_plus, std::make_shared<AdherenceData>(partial_plus_prob, AdherenceCategory::PARTIAL_PLUS));
	creator.addItem(partial_minus, std::make_shared<AdherenceData>(partial_minus_prob, AdherenceCategory::PARTIAL_MINUS));

	std::shared_ptr<AdherenceData> data = creator.createProbDist().draw(repast::Random::instance()->nextDouble());
	person->setArtAdherence({data->probability, data->category});
	schedule_art_adherence(person, first_art_at_tick);
}

void schedule_art_adherence(std::shared_ptr<Person> person, double first_art_at_tick) {
	double adherence_check_at = first_art_at_tick
			+ Parameters::instance()->getDoubleParameter(PARTIAL_ART_ADHER_WINDOW_LENGTH);
	ARTAdherenceCheckScheduler* ac_scheduler = new ARTAdherenceCheckScheduler(person, adherence_check_at);
	repast::RepastProcess::instance()->getScheduleRunner().scheduleEvent(adherence_check_at - 0.1,
			repast::Schedule::FunctorPtr(ac_scheduler));
}

AdherenceData initialize_prep_adherence(AdherenceCategory category) {
	double prob = 0;
	if (category == AdherenceCategory::ALWAYS) {
		prob = Parameters::instance()->getDoubleParameter(PREP_ALWAYS_ADHERENT_TR);
	} else if (category == AdherenceCategory::NEVER) {
		prob = Parameters::instance()->getDoubleParameter(PREP_NEVER_ADHERENT_TR);
	} else if (category == AdherenceCategory::PARTIAL_PLUS) {
		prob = Parameters::instance()->getDoubleParameter(PREP_PARTIAL_POS_ADHERENT_TR);
	}  else if (category == AdherenceCategory::PARTIAL_MINUS) {
		prob = Parameters::instance()->getDoubleParameter(PREP_PARTIAL_NEG_ADHERENT_TR);
	}

	return  AdherenceData(prob, category);
}

AdherenceData initialize_prep_adherence() {
	double always = Parameters::instance()->getDoubleParameter(PREP_PROP_ALWAYS_ADHERENT);
	double never = Parameters::instance()->getDoubleParameter(PREP_PROP_NEVER_ADHERENT);
	double partial_plus = Parameters::instance()->getDoubleParameter(PREP_PROP_PARTIAL_POS_ADHERENT);
	double partial_minus = Parameters::instance()->getDoubleParameter(PREP_PROP_PARTIAL_NEG_ADHERENT);

	double always_prob = Parameters::instance()->getDoubleParameter(PREP_ALWAYS_ADHERENT_TR);
	double never_prob = Parameters::instance()->getDoubleParameter(PREP_NEVER_ADHERENT_TR);
	double partial_plus_prob = Parameters::instance()->getDoubleParameter(PREP_PARTIAL_POS_ADHERENT_TR);
	double partial_minus_prob = Parameters::instance()->getDoubleParameter(PREP_PARTIAL_NEG_ADHERENT_TR);

	//std::cout << "prep: " << always << "," << never << "," << partial_plus << ", " << partial_minus << std::endl;

	ProbDistCreator<AdherenceData> creator;
	creator.addItem(always, std::make_shared<AdherenceData>(always_prob, AdherenceCategory::ALWAYS));
	creator.addItem(never, std::make_shared<AdherenceData>(never_prob, AdherenceCategory::NEVER));
	creator.addItem(partial_plus, std::make_shared<AdherenceData>(partial_plus_prob, AdherenceCategory::PARTIAL_PLUS));
	creator.addItem(partial_minus, std::make_shared<AdherenceData>(partial_minus_prob, AdherenceCategory::PARTIAL_MINUS));

	std::shared_ptr<AdherenceData> data = creator.createProbDist().draw(repast::Random::instance()->nextDouble());
	return AdherenceData(data->probability, data->category);
}

}

