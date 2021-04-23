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

class MethPrepFilter : public PrepFilter {

public:
    MethPrepFilter();
    virtual ~MethPrepFilter();

    bool apply(PersonPtr person) override;
    double calcPrepStopAdjustment() override;
};


class CrackPrepFilter : public PrepFilter {

public:
    CrackPrepFilter();
    virtual ~CrackPrepFilter();

    bool apply(PersonPtr person) override;
    double calcPrepStopAdjustment() override;
};

class EcstasyPrepFilter : public PrepFilter {

public:
    EcstasyPrepFilter();
    virtual ~EcstasyPrepFilter();

    bool apply(PersonPtr person) override;
    double calcPrepStopAdjustment() override;
};

class MethCrackPrepFilter : public PrepFilter {

public:
    MethCrackPrepFilter();
    virtual ~MethCrackPrepFilter();

    bool apply(PersonPtr person) override;
    double calcPrepStopAdjustment() override;
};

class MethEcstasyPrepFilter : public PrepFilter {

public:
    MethEcstasyPrepFilter();
    virtual ~MethEcstasyPrepFilter();

    bool apply(PersonPtr person) override;
    double calcPrepStopAdjustment() override;
};

class CrackEcstasyPrepFilter : public PrepFilter {

public:
    CrackEcstasyPrepFilter();
    virtual ~CrackEcstasyPrepFilter();

    bool apply(PersonPtr person) override;
    double calcPrepStopAdjustment() override;
};

class MethCrackEcstasyPrepFilter : public PrepFilter {

public:
    MethCrackEcstasyPrepFilter();
    virtual ~MethCrackEcstasyPrepFilter();

    bool apply(PersonPtr person) override;
    double calcPrepStopAdjustment() override;
};

class MirtazapinePrepFilter : public PrepFilter {

public:
    MirtazapinePrepFilter();
    virtual ~MirtazapinePrepFilter();

    bool apply(PersonPtr person) override;
    double calcPrepStopAdjustment() override;
};

class BCPrepFilter : public PrepFilter {

public:
    BCPrepFilter();
    virtual ~BCPrepFilter();

    bool apply(PersonPtr person) override;
    double calcPrepStopAdjustment() override;
};

}


#endif
