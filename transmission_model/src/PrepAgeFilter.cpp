/**
 * PrepAgeFilter.cpp
 */

#include "Person.h"
#include "PrepAgeFilter.h"

namespace TransModel {


PrepAgeFilter::PrepAgeFilter(double age_threshold) : age_threshold_(age_threshold) {}

PrepAgeFilter::~PrepAgeFilter() {}

LTPrepAgeFilter::LTPrepAgeFilter(double age_threshold) : PrepAgeFilter(age_threshold) {}
LTPrepAgeFilter::~LTPrepAgeFilter() {}

bool LTPrepAgeFilter::apply(PersonPtr person) {
    return person->age() < age_threshold_;
}

double LTPrepAgeFilter::calcPrepStopAdjustment() { 
    return 0;
}


GTEPrepAgeFilter::GTEPrepAgeFilter(double age_threshold, double max_age) : PrepAgeFilter(age_threshold), gte_adjustment((1.0 / 365) * (1 / (max_age - age_threshold))) {}

GTEPrepAgeFilter::~GTEPrepAgeFilter() {}

bool GTEPrepAgeFilter::apply(PersonPtr person) {
    return person->age() >= age_threshold_;
}
double GTEPrepAgeFilter::calcPrepStopAdjustment() {
    return gte_adjustment;
}

}