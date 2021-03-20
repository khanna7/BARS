#include <random>
#include "gtest/gtest.h"

#include "repast_hpc/RepastProcess.h"

#include "Parameters.h"
#include "RInstance.h"
#include "utils.h"


using namespace TransModel;
using namespace std;

class SubstanceUseTests: public ::testing::Test {

protected:

    SubstanceUseTests() {
        repast::Random::initialize(1);
        repast::RepastProcess::init("");

        repast::Properties props("../test_data/chi_model.props");
        props.putProperty("global.random.seed", "1");
        Parameters::initialize(props);
        init_parameters("../test_data/chi_params_nonderived.R", "../test_data/chi_params_derived.R", "", Parameters::instance(),
               RInstance::rptr);
    }

    virtual ~SubstanceUseTests() {
    }
};
TEST_F(SubstanceUseTests, TestARTAdherenceProportions) {
    std::string suffix;
    double always, never, partial_plus, partial_minus;
    double error = 0.000000001;

    suffix = "";
    suffix += METH_SUFFIX;
    suffix += CRACK_SUFFIX;
    suffix += ECSTASY_SUFFIX;
    always = Parameters::instance()->getDoubleParameter(ART_PROP_ALWAYS_ADHERENT + suffix);
    never = Parameters::instance()->getDoubleParameter(ART_PROP_NEVER_ADHERENT + suffix);
    partial_plus = Parameters::instance()->getDoubleParameter(ART_PROP_PARTIAL_POS_ADHERENT + suffix);
    partial_minus = Parameters::instance()->getDoubleParameter(ART_PROP_PARTIAL_NEG_ADHERENT + suffix);

    ASSERT_NEAR(1.0, always+never+partial_plus+partial_minus, error);

    suffix = "";
    suffix += METH_SUFFIX;

    always = Parameters::instance()->getDoubleParameter(ART_PROP_ALWAYS_ADHERENT + suffix);
    never = Parameters::instance()->getDoubleParameter(ART_PROP_NEVER_ADHERENT + suffix);
    partial_plus = Parameters::instance()->getDoubleParameter(ART_PROP_PARTIAL_POS_ADHERENT + suffix);
    partial_minus = Parameters::instance()->getDoubleParameter(ART_PROP_PARTIAL_NEG_ADHERENT + suffix);

    ASSERT_NEAR(1.0, always+never+partial_plus+partial_minus, error);
    
    suffix = "";
    suffix += CRACK_SUFFIX;

    always = Parameters::instance()->getDoubleParameter(ART_PROP_ALWAYS_ADHERENT + suffix);
    never = Parameters::instance()->getDoubleParameter(ART_PROP_NEVER_ADHERENT + suffix);
    partial_plus = Parameters::instance()->getDoubleParameter(ART_PROP_PARTIAL_POS_ADHERENT + suffix);
    partial_minus = Parameters::instance()->getDoubleParameter(ART_PROP_PARTIAL_NEG_ADHERENT + suffix);

    ASSERT_NEAR(1.0, always+never+partial_plus+partial_minus, error);
    
    suffix = "";
    suffix += ECSTASY_SUFFIX;

    always = Parameters::instance()->getDoubleParameter(ART_PROP_ALWAYS_ADHERENT + suffix);
    never = Parameters::instance()->getDoubleParameter(ART_PROP_NEVER_ADHERENT + suffix);
    partial_plus = Parameters::instance()->getDoubleParameter(ART_PROP_PARTIAL_POS_ADHERENT + suffix);
    partial_minus = Parameters::instance()->getDoubleParameter(ART_PROP_PARTIAL_NEG_ADHERENT + suffix);

    ASSERT_NEAR(1.0, always+never+partial_plus+partial_minus, error);
    
    suffix = "";
    suffix += METH_SUFFIX;
    suffix += CRACK_SUFFIX;

    always = Parameters::instance()->getDoubleParameter(ART_PROP_ALWAYS_ADHERENT + suffix);
    never = Parameters::instance()->getDoubleParameter(ART_PROP_NEVER_ADHERENT + suffix);
    partial_plus = Parameters::instance()->getDoubleParameter(ART_PROP_PARTIAL_POS_ADHERENT + suffix);
    partial_minus = Parameters::instance()->getDoubleParameter(ART_PROP_PARTIAL_NEG_ADHERENT + suffix);

    ASSERT_NEAR(1.0, always+never+partial_plus+partial_minus, error);
    
    suffix = "";
    suffix += METH_SUFFIX;
    suffix += ECSTASY_SUFFIX;

    always = Parameters::instance()->getDoubleParameter(ART_PROP_ALWAYS_ADHERENT + suffix);
    never = Parameters::instance()->getDoubleParameter(ART_PROP_NEVER_ADHERENT + suffix);
    partial_plus = Parameters::instance()->getDoubleParameter(ART_PROP_PARTIAL_POS_ADHERENT + suffix);
    partial_minus = Parameters::instance()->getDoubleParameter(ART_PROP_PARTIAL_NEG_ADHERENT + suffix);

    ASSERT_NEAR(1.0, always+never+partial_plus+partial_minus, error);
    
    suffix = "";
    suffix += CRACK_SUFFIX;
    suffix += ECSTASY_SUFFIX;

    always = Parameters::instance()->getDoubleParameter(ART_PROP_ALWAYS_ADHERENT + suffix);
    never = Parameters::instance()->getDoubleParameter(ART_PROP_NEVER_ADHERENT + suffix);
    partial_plus = Parameters::instance()->getDoubleParameter(ART_PROP_PARTIAL_POS_ADHERENT + suffix);
    partial_minus = Parameters::instance()->getDoubleParameter(ART_PROP_PARTIAL_NEG_ADHERENT + suffix);

    ASSERT_NEAR(1.0, always+never+partial_plus+partial_minus, error);
}
