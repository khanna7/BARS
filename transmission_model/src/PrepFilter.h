#ifndef SRC_PREPFILTER_H
#define SRC_PREPFILTER_H

#include "common.h"

#include <boost/mpl/map/aux_/at_impl.hpp>

namespace TransModel {

class PrepFilter {

public:
    PrepFilter();
    virtual ~PrepFilter();

    virtual bool apply(PersonPtr person) = 0;
    virtual double calcPrepStopAdjustment() = 0;
};

class LTAgePrepFilter : public PrepFilter {

private:
    double age_threshold_;

public:
    LTAgePrepFilter(double age_threshold);
    virtual ~LTAgePrepFilter();

    bool apply(PersonPtr person) override;
    double calcPrepStopAdjustment() override;
};

class GTEAgePrepFilter : public PrepFilter {

private:
    double age_threshold_;
    double gte_adjustment;

public:
    GTEAgePrepFilter(double age_threshold, double max_age);
    virtual ~GTEAgePrepFilter();

    bool apply(PersonPtr person) override;
    double calcPrepStopAdjustment() override;
};

class NonSubstanceUsePrepFilter : public PrepFilter {

public:
    NonSubstanceUsePrepFilter();
    virtual ~NonSubstanceUsePrepFilter();

    bool apply(PersonPtr person) override;
    double calcPrepStopAdjustment() override;
};

class PolystimulantUsePrepFilter : public PrepFilter {

public:
    PolystimulantUsePrepFilter();
    virtual ~PolystimulantUsePrepFilter();

    bool apply(PersonPtr person) override;
    double calcPrepStopAdjustment() override;
};

}

#endif
