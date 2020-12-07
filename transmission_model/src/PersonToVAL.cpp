#include "PersonToVAL.h"
#include "Diagnoser.h"
#include "Person.h"

using namespace Rcpp;

namespace TransModel {

List PersonToVAL::operator()(const PersonPtr& p, int idx, double tick) const {
    List vertex = List::create();

    vertex["na"] = false, vertex["vertex_names"] = idx;

    vertex[C_ID] = p->id();
    vertex["age"] = p->age();
    vertex["cd4.count.today"] = p->infectionParameters().cd4_count;
    vertex["circum.status"] = p->isCircumcised();

    vertex["meth.user"] = p->isSubstanceUser(SubstanceUseType::METH);
    vertex["crack.user"] = p->isSubstanceUser(SubstanceUseType::CRACK);
    vertex["ecstasy.user"] = p->isSubstanceUser(SubstanceUseType::ECSTASY);

    vertex["diagnosed"] = p->isDiagnosed();
    if (p->isDiagnosed()) {
        vertex["time_of_diagnosis"] = p->infectionParameters().time_of_diagnosis;
        vertex["time_since_diagnosed"] = p->infectionParameters().time_since_diagnosed;
        vertex["art_lag"] = p->infectionParameters().art_lag;
    } else {
        vertex["time_of_diagnosis"] = NA_REAL;
        vertex["time_since_diagnosed"] = NA_REAL;
        vertex["art_lag"] = NA_REAL;
    }
    const Diagnoser& diagnoser = p->diagnoser();
    vertex["number.of.tests"] = diagnoser.testCount();
    vertex["testing.probability"] = diagnoser.testingProbability();
    vertex["non.testers"] = !(p->isTestable());
    vertex["role_casual"] = p->casual_role();
    vertex["role_main"] = p->steady_role();

    vertex["prep.status"] = static_cast<int>(p->prepStatus());

    if (p->isOnPrep(false)) {
        vertex["time.of.prep.cessation"] = p->prepParameters().stopTime();
        vertex["time.of.prep.initiation"] = p->prepParameters().startTime();
    }

    if (p->isInfected()) {
        vertex["infectivity"] = p->infectivity();
        vertex["art.status"] = p->isOnART(false);
        vertex["inf.status"] = p->isInfected();
        vertex["time.since.infection"] = p->infectionParameters().time_since_infection;
        vertex["time.of.infection"] = p->infectionParameters().time_of_infection;
        vertex["age.at.infection"] = p->infectionParameters().age_at_infection;
        vertex["viral.load.today"] = p->infectionParameters().viral_load;
    } else {
        vertex["infectivity"] = 0;
        vertex["art.covered"] = NA_LOGICAL;
        vertex["art.status"] = NA_LOGICAL;
        vertex["inf.status"] = false;
        vertex["time.since.infection"] = NA_REAL;
        vertex["time.of.infection"] = NA_REAL;
        vertex["age.at.infection"] = NA_REAL;
        vertex["viral.load.today"] = 0;
    }

    vertex["adherence.category"] = static_cast<int>(p->artAdherence().category);
    vertex["prep.adherence.category"] = static_cast<int>(p->prepParameters().adherenceCagegory());

    if (p->isDiagnosed()) {
        // if diagnosed, could have started ART 
        // but check for NAN's if diagnosed but not yet on ART, i.e. in lag period
        vertex["time.since.art.initiation"] = isnan(p->infectionParameters().time_since_art_init) ? NA_REAL : p->infectionParameters().time_since_art_init;
        vertex["time.of.art.initiation"] = isnan(p->infectionParameters().time_of_art_init) ? NA_REAL : p->infectionParameters().time_of_art_init;
        vertex["vl.art.traj.slope"] = isnan(p->infectionParameters().vl_art_traj_slope) ? NA_REAL : p->infectionParameters().vl_art_traj_slope;
        vertex["cd4.at.art.initiation"] = isnan(p->infectionParameters().cd4_at_art_init) ? NA_REAL : p->infectionParameters().cd4_at_art_init;
        vertex["vl.at.art.initiation"] = isnan(p->infectionParameters().vl_at_art_init) ? NA_REAL : p->infectionParameters().vl_at_art_init;
    } else {
        // probably don't need this branch
        vertex["time.since.art.initiation"] = NA_REAL;
        vertex["time.of.art.initiation"] = NA_REAL;
        vertex["vl.art.traj.slope"] = NA_REAL;
        vertex["cd4.at.art.initiation"] = NA_REAL;
        vertex["vl.at.art.initiation"] = NA_REAL;
    }

    const JailParameters& jp = p->jailParameters();
    vertex["is.in.jail"] = jp.is_in_jail;
    vertex["is.first.time.jailed"] = jp.is_first_time_jailed;
    vertex["time.of.jail"] = jp.time_of_jail;
    vertex["serving.time"] = jp.serving_time;
    vertex["age.at.jail"] = jp.age_at_jail;
    vertex["time.since.jailed"] = isnan(jp.time_since_jailed) ? NA_REAL : jp.time_since_jailed;
    vertex["accumulative.injail.count"] = jp.accumulative_injail_count;
    vertex["accumulative.time.in.jail"] = jp.accumulative_time_in_jail;
    vertex["art.forced.off"] = p->infectionParameters().art_forced_off;
    vertex["prep.forced.off"] = p->prepParameters().prepForcedOff();
    
    return vertex;
}

}
