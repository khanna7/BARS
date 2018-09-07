#ifndef SRC_PREPAGEFILTER_H
#define SRC_PREPAGEFILTER_H

#include "common.h"

namespace TransModel {

class PrepAgeFilter {

protected:
    double age_threshold_;

public:
    PrepAgeFilter(double age_threshold);
    virtual ~PrepAgeFilter();

    virtual bool apply(PersonPtr person) = 0;
    virtual double calcPrepStopAdjustment() = 0;
};

class LTPrepAgeFilter : public PrepAgeFilter {

public:
    LTPrepAgeFilter(double age_threshold);
    virtual ~LTPrepAgeFilter();

    bool apply(PersonPtr person) override;
    double calcPrepStopAdjustment() override;
};

class GTEPrepAgeFilter : public PrepAgeFilter {

private:
    double gte_adjustment;

public:
    GTEPrepAgeFilter(double age_threshold, double max_age);
    virtual ~GTEPrepAgeFilter();

    bool apply(PersonPtr person) override;
    double calcPrepStopAdjustment() override;
};


}

#endif