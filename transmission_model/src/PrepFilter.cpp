/**
 * PrepAgeFilter.cpp
 */

#include "Person.h"
#include "PrepFilter.h"

namespace TransModel {


PrepFilter::PrepFilter() {}

PrepFilter::~PrepFilter() {}

LTAgePrepFilter::LTAgePrepFilter(double age_threshold) : age_threshold_(age_threshold) {}
LTAgePrepFilter::~LTAgePrepFilter() {}

bool LTAgePrepFilter::apply(PersonPtr person) {
    return person->age() < age_threshold_;
}

double LTAgePrepFilter::calcPrepStopAdjustment() { 
    return 0;
}


GTEAgePrepFilter::GTEAgePrepFilter(double age_threshold, double max_age) : age_threshold_(age_threshold), gte_adjustment((1.0 / 365) * (1 / (max_age - age_threshold))) {}

GTEAgePrepFilter::~GTEAgePrepFilter() {}

bool GTEAgePrepFilter::apply(PersonPtr person) {
    return person->age() >= age_threshold_;
}

double GTEAgePrepFilter::calcPrepStopAdjustment() {
    return gte_adjustment;
}

NonSubstanceUsePrepFilter::NonSubstanceUsePrepFilter() {}

NonSubstanceUsePrepFilter::~NonSubstanceUsePrepFilter() {}

bool NonSubstanceUsePrepFilter::apply(PersonPtr person) {
    return !person->isSubstanceUser();
}

double NonSubstanceUsePrepFilter::calcPrepStopAdjustment() {
    return 0;
}

MethPrepFilter::MethPrepFilter() {}

MethPrepFilter::~MethPrepFilter() {}

bool MethPrepFilter::apply(PersonPtr person) {
    return person->isSubstanceUser(SubstanceUseType::METH) &&
            !person->isSubstanceUser(SubstanceUseType::CRACK) &&
            !person->isSubstanceUser(SubstanceUseType::ECSTASY);
}

double MethPrepFilter::calcPrepStopAdjustment() {
    return 0;
}

CrackPrepFilter::CrackPrepFilter() {}

CrackPrepFilter::~CrackPrepFilter() {}

bool CrackPrepFilter::apply(PersonPtr person) {
    return person->isSubstanceUser(SubstanceUseType::CRACK) &&
            !person->isSubstanceUser(SubstanceUseType::METH) &&
            !person->isSubstanceUser(SubstanceUseType::ECSTASY);
}

double CrackPrepFilter::calcPrepStopAdjustment() {
    return 0;
}

EcstasyPrepFilter::EcstasyPrepFilter() {}

EcstasyPrepFilter::~EcstasyPrepFilter() {}

bool EcstasyPrepFilter::apply(PersonPtr person) {
    return person->isSubstanceUser(SubstanceUseType::ECSTASY) &&
            !person->isSubstanceUser(SubstanceUseType::METH) &&
            !person->isSubstanceUser(SubstanceUseType::CRACK);
}

double EcstasyPrepFilter::calcPrepStopAdjustment() {
    return 0;
}

MethCrackPrepFilter::MethCrackPrepFilter() {}

MethCrackPrepFilter::~MethCrackPrepFilter() {}

bool MethCrackPrepFilter::apply(PersonPtr person) {
    return !person->isSubstanceUser(SubstanceUseType::ECSTASY) &&
            person->isSubstanceUser(SubstanceUseType::METH) &&
            person->isSubstanceUser(SubstanceUseType::CRACK);
}

double MethCrackPrepFilter::calcPrepStopAdjustment() {
    return 0;
}

MethEcstasyPrepFilter::MethEcstasyPrepFilter() {}

MethEcstasyPrepFilter::~MethEcstasyPrepFilter() {}

bool MethEcstasyPrepFilter::apply(PersonPtr person) {
    return person->isSubstanceUser(SubstanceUseType::ECSTASY) &&
            person->isSubstanceUser(SubstanceUseType::METH) &&
            !person->isSubstanceUser(SubstanceUseType::CRACK);
}

double MethEcstasyPrepFilter::calcPrepStopAdjustment() {
    return 0;
}

CrackEcstasyPrepFilter::CrackEcstasyPrepFilter() {}

CrackEcstasyPrepFilter::~CrackEcstasyPrepFilter() {}

bool CrackEcstasyPrepFilter::apply(PersonPtr person) {
    return person->isSubstanceUser(SubstanceUseType::ECSTASY) &&
            !person->isSubstanceUser(SubstanceUseType::METH) &&
            person->isSubstanceUser(SubstanceUseType::CRACK);
}

double CrackEcstasyPrepFilter::calcPrepStopAdjustment() {
    return 0;
}

MethCrackEcstasyPrepFilter::MethCrackEcstasyPrepFilter() {}

MethCrackEcstasyPrepFilter::~MethCrackEcstasyPrepFilter() {}

bool MethCrackEcstasyPrepFilter::apply(PersonPtr person) {
    return person->isSubstanceUser(SubstanceUseType::ECSTASY) &&
            person->isSubstanceUser(SubstanceUseType::METH) &&
            person->isSubstanceUser(SubstanceUseType::CRACK);
}

double MethCrackEcstasyPrepFilter::calcPrepStopAdjustment() {
    return 0;
}

MirtazapinePrepFilter::MirtazapinePrepFilter() {}

MirtazapinePrepFilter::~MirtazapinePrepFilter() {}

bool MirtazapinePrepFilter::apply(PersonPtr person) {
    return !person->adheringToMirtazapineTreatment();
}

double MirtazapinePrepFilter::calcPrepStopAdjustment() {
    return 0;
}

BCPrepFilter::BCPrepFilter() {}

BCPrepFilter::~BCPrepFilter() {}

bool BCPrepFilter::apply(PersonPtr person) {
    return !person->onCounselingAndBehavioralTreatment();
}

double BCPrepFilter::calcPrepStopAdjustment() {
    return 0;
}

}
