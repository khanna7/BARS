/*
 * CD4ViralTests.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: nick
 */

#include "gtest/gtest.h"

#include "repast_hpc/RepastProcess.h"

#include "Parameters.h"
#include "PersonCreator.h"
#include "RInstance.h"
#include "TransmissionRunner.h"
#include "StatsBuilder.h"
#include "utils.h"
#include "Network.h"

using namespace TransModel;
using namespace Rcpp;

class CreatorTests: public ::testing::Test {

protected:

    CreatorTests() {

        repast::Random::initialize(1);
        repast::RepastProcess::init("");

        StatsBuilder builder("/dev");
        builder.countsWriter("null");
        builder.partnershipEventWriter("null");
        builder.infectionEventWriter("null");
        builder.biomarkerWriter("null");
        builder.deathEventWriter("null");
        builder.personDataRecorder("null");
        builder.testingEventWriter("null");
        builder.prepEventWriter("null");
        builder.artEventWriter("null");
        builder.createStatsSingleton(16, 36);

        repast::Properties props("../test_data/chi_model.props");
        Parameters::initialize(props);
        //RInstance::rptr->parseEvalQ("rm(list=ls()");
        init_parameters("../test_data/chi_params_nonderived.R", "../test_data/chi_params_derived.R", "", Parameters::instance(),
            RInstance::rptr);
        
        float circum_rate = Parameters::instance()->getDoubleParameter(CIRCUM_RATE);
        BinomialGen rate(repast::Random::instance()->engine(), boost::random::binomial_distribution<>(1, circum_rate));
        repast::Random::instance()->putGenerator(CIRCUM_STATUS_BINOMIAL, new repast::DefaultNumberGenerator<BinomialGen>(rate));
    }

    virtual ~CreatorTests() {
    }
};

ARTLagCalculator create_art_lag_calc() {
    DayRangeCalculatorCreator creator;
    vector<string> lag_keys;
    Parameters::instance()->getKeys(ART_LAG_PREFIX_LT, lag_keys);
    for (auto& key : lag_keys) {
        std::cout << key << ": " << Parameters::instance()->getStringParameter(key) << std::endl;
        creator.addBin(Parameters::instance()->getStringParameter(key));
    }
    std::shared_ptr<DayRangeCalculator> lower = creator.createCalculator();

    creator.clear();
    lag_keys.clear();
    Parameters::instance()->getKeys(ART_LAG_PREFIX_GTE, lag_keys);
    for (auto& key : lag_keys) {
        std::cout << key << ": " << Parameters::instance()->getStringParameter(key) << std::endl;
        creator.addBin(Parameters::instance()->getStringParameter(key));
    }
    std::shared_ptr<DayRangeCalculator> upper = creator.createCalculator();

    float age_threshold =  Parameters::instance()->getFloatParameter(INPUT_AGE_THRESHOLD);
    return ARTLagCalculator(upper, lower, age_threshold);
}

// TEST_F(CreatorTests, TestInfectedPersonCreationNoART) {
//     std::string cmd = "load(file=\"../test_data/chi_net_n5000.RData\")";
//     RInstance::rptr->parseEvalQ(cmd);
//     List rnet = as<List>((*RInstance::rptr)["n0"]);
//     List val = as<List>(rnet["val"]);
//     List p_list = as<List>(val[101]);
//     // make sure I have the right one
//     ASSERT_NEAR(61.87456, as<double>(p_list["age"]), 0.00001);

//     std::shared_ptr<TransmissionRunner> runner = std::make_shared<TransmissionRunner>(1, 1, 1, 10);
//     PersonCreator creator(runner, 1, create_art_lag_calc());
//     PersonPtr person = creator(p_list, 1, 0);
//     ASSERT_FALSE(person->isCircumcised());
//     ASSERT_TRUE(person->isInfected());
//     ASSERT_EQ(0, person->steady_role());
//     ASSERT_NEAR(61.87456f, person->age(), 0.00001);
//     ASSERT_FALSE(person->isDiagnosed());
//     ASSERT_TRUE(person->isTestable());
//     ASSERT_FALSE(person->isOnPrep());

//     InfectionParameters params = person->infectionParameters();
//     ASSERT_FALSE(params.art_status);
//     ASSERT_FLOAT_EQ(294, params.time_since_infection);
//     ASSERT_FLOAT_EQ(-294, params.time_of_infection);
//     ASSERT_FLOAT_EQ(61.87456, params.age_at_infection);
//     ASSERT_FLOAT_EQ(455.2354, params.cd4_count);
//     ASSERT_TRUE(isnan(params.time_since_art_init));
//     ASSERT_TRUE(isnan(params.time_of_art_init));
//     ASSERT_FLOAT_EQ(4.2f, params.viral_load);
//     ASSERT_TRUE(isnan(params.vl_art_traj_slope));
//     ASSERT_TRUE(isnan(params.vl_at_art_init));
//     ASSERT_TRUE(isnan(params.cd4_at_art_init));
//}

TEST_F(CreatorTests, TestUninfectedPersonCreation) {
    std::string cmd = "load(file=\"../test_data/chi_net_n5000.RData\")";
    RInstance::rptr->parseEvalQ(cmd);
    List rnet = as<List>((*RInstance::rptr)["n0"]);
    List val = as<List>(rnet["val"]);
    List p_list = as<List>(val[1]);
    // make sure I have the right one
    ASSERT_NEAR(18.97795, as<double>(p_list["age"]), 0.00001);

    Network<Person> net;
    JailInfRateCalculator calc(3, 1, 0);
    Jail jail(&net, calc);

    std::shared_ptr<TransmissionRunner> runner = std::make_shared<TransmissionRunner>(1, 1, 1, 10);
    PersonCreator creator(runner, 1, create_art_lag_calc(), &jail);
    PersonPtr person = creator(p_list, 1, 0);
    ASSERT_TRUE(person->isCircumcised());
    ASSERT_FALSE(person->isInfected());
    ASSERT_EQ(0, person->steady_role());
    ASSERT_NEAR(18.97795f, person->age(), 0.00001);
    ASSERT_TRUE(person->isTestable());
    ASSERT_FALSE(person->isOnPrep(false));

    InfectionParameters params = person->infectionParameters();
    ASSERT_FALSE(params.art_status);
    ASSERT_TRUE(isnan(params.time_since_infection));
    ASSERT_TRUE(isnan(params.time_of_infection));
    ASSERT_TRUE(isnan(params.age_at_infection));
    ASSERT_FLOAT_EQ(916.0f, params.cd4_count);
    ASSERT_TRUE(isnan(params.time_since_infection));
    ASSERT_TRUE(isnan(params.time_since_art_init));
    ASSERT_TRUE(isnan(params.time_of_art_init));
    ASSERT_FLOAT_EQ(0, params.viral_load);
    ASSERT_TRUE(isnan(params.vl_art_traj_slope));
    ASSERT_TRUE(isnan(params.vl_at_art_init));
    ASSERT_TRUE(isnan(params.cd4_at_art_init));
}

// TEST_F(CreatorTests, TestInfectedPersonCreationART) {
//     std::string cmd = "load(file=\"../test_data/chi_net_n5000.RData\")";
//     RInstance::rptr->parseEvalQ(cmd);
//     List rnet = as<List>((*RInstance::rptr)["n0"]);
//     List val = as<List>(rnet["val"]);
//     List p_list = as<List>(val[9]);
//     // make sure I have the right one
//     ASSERT_NEAR(19.45746, as<double>(p_list["age"]), 0.00001);

//     std::shared_ptr<TransmissionRunner> runner = std::make_shared<TransmissionRunner>(1, 1, 1, 10);
//     PersonCreator creator(runner, 1, create_art_lag_calc());
//     PersonPtr person = creator(p_list, 1, 0);
//     ASSERT_FALSE(person->isCircumcised());
//     ASSERT_TRUE(person->isInfected());
//     ASSERT_EQ(0, person->steady_role());
//     ASSERT_NEAR(19.45746f, person->age(), 0.00001);

//     ASSERT_TRUE(person->isDiagnosed());
//     ASSERT_TRUE(person->isTestable());
//     ASSERT_FALSE(person->isOnPrep());

//     InfectionParameters params = person->infectionParameters();
//     ASSERT_TRUE(params.art_status);
//     ASSERT_FLOAT_EQ(2274, params.time_since_infection);
//     ASSERT_FLOAT_EQ(-2274, params.time_of_infection);
//     ASSERT_FLOAT_EQ(19.45746f, params.age_at_infection);
//     ASSERT_FLOAT_EQ(243.5267f, params.cd4_count);
//     ASSERT_EQ(0, params.time_since_art_init);
//     ASSERT_EQ(0, params.time_of_art_init);
//     ASSERT_FLOAT_EQ(4.2f, params.viral_load);
//     ASSERT_NEAR(0.02084192f, params.vl_art_traj_slope, 0.000001);
//     ASSERT_FLOAT_EQ(4.2f, params.vl_at_art_init);
//     ASSERT_FLOAT_EQ(0, params.cd4_at_art_init);
// }

// TEST_F(CreatorTests, TestCreatorFromSavedNet) {
//     std::string cmd = "nw.test <- readRDS(file=\"../test_data/network.RDS\")";
//     RInstance::rptr->parseEvalQ(cmd);
//     List rnet = as<List>((*RInstance::rptr)["n0"]);

//     std::shared_ptr<TransmissionRunner> runner = std::make_shared<TransmissionRunner>(1, 1, 1, 10);
//     PersonCreator creator(runner, 1, create_art_lag_calc());
//     List val = as<List>(rnet["val"]);

//     // will throw exception if there's an issue
//     for (auto& sexp : val) {
//         List v = as<List>(sexp);
//         creator(v, 1, 0);
//     }
// }



// TEST_F(CreatorTests, TestDiagnosis) {
//     std::string cmd = "load(file=\"../test_data/initialized-model.RData\")";
//     RInstance::rptr->parseEvalQ(cmd);
//     List rnet = as<List>((*RInstance::rptr)["n0"]);
//     List val = as<List>(rnet["val"]);
//     List p_list = as<List>(val[0]);
//     p_list["time.until.next.test"] = 528;
//     p_list["lag.bet.diagnosis.and.art.init"] = 0;

//     std::shared_ptr<TransmissionRunner> runner = std::make_shared<TransmissionRunner>(1, 1, 1, 10);

//     // detection window = 10
//     PersonCreator creator(runner, 10, create_art_lag_calc());
//     double tick = 2;
//     PersonPtr person = creator(p_list, tick, 0);

//     float next_test_at = tick + as<double>(p_list["time.until.next.test"]);
//     ASSERT_EQ(528 + tick, next_test_at);
//     person->infect(50, tick);
// //	// person should diagnose true at next_text_at, detection window is 10 which is long
// //	// before next_test_at
//     Stats::instance()->personDataRecorder()->initRecord(person, 0);
//     ASSERT_FALSE(person->diagnose(100));
//     ASSERT_FALSE(person->diagnose(next_test_at - 1));
//     ASSERT_TRUE(person->diagnose(next_test_at));

//     p_list["time.until.next.test"] = 5;
//     PersonPtr p2 = creator(p_list, tick, 0);
//     Stats::instance()->personDataRecorder()->initRecord(p2, 0);
//     p2->infect(50, tick);
//     // test should be false at next_test_at (7)
//     // because detection window not yet reached
//     ASSERT_FALSE(p2->diagnose(7));
//     // do the next test at the max of the next testing data
//     // or the detection window to insure a positive test
//     double test_at = std::max(7 + next_test_at, 12.0f);
//     ASSERT_TRUE(p2->diagnose(test_at));
//     ASSERT_TRUE(p2->isDiagnosed());
// }
