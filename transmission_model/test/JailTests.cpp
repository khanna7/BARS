
#include <random>
#include "gtest/gtest.h"


#include "repast_hpc/RepastProcess.h"

#include "Jail.h"
#include "JailInfectionRateCalc.h"
#include "Parameters.h"
#include "PersonCreator.h"
#include "StatsBuilder.h"
#include "utils.h"
#include "CondomUseAssigner.h"
#include "Network.h"
#include "network_utils.h"

#include "RInstance.h"
#include "Rcpp.h"

using namespace TransModel;
using namespace std;

class JailTests: public ::testing::Test {

protected:

    JailTests() {
        std::string cmd = "source(file=\"../test_data/network_tests.R\")";
        RInstance::rptr->parseEvalQ(cmd);

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
        props.putProperty("global.random.seed", "1");
        Parameters::initialize(props);
    }

    virtual ~JailTests() {
    }
};


TEST_F(JailTests, TestJailNetworkRetention) {
    Diagnoser diagnoser(5, 0, 1);
    
    PersonPtr p1 = make_shared<Person>(1, 10, true, 0, 1, diagnoser);
    PersonPtr p2 = make_shared<Person>(2, 10, true, 0, 1, diagnoser);
    PersonPtr p3 = make_shared<Person>(3, 10, true, 0, 1, diagnoser);
    PersonPtr p4 = make_shared<Person>(4, 10, true, 0, 1, diagnoser);

    Network<Person> net;
    net.addVertex(p1);
    net.addVertex(p2);
    net.addVertex(p3);
    net.addVertex(p4);
    net.addEdge(p2, p1, 0);
    
    JailInfRateCalculator calc(0, 0, 0);
    Jail jail(&net, calc);
    p2->goOnPrep(1, 2);
    ASSERT_TRUE(p2->isOnPrep(true));
    ASSERT_TRUE(p2->isOnPrep(false));
    jail.addPerson(1, p2);

    ASSERT_FALSE(net.contains(p2));
    ASSERT_TRUE(p2->isJailed());
    ASSERT_FALSE(net.hasEdge(p2, p1, 0));
    // care disruption is off so still on PreP.
    ASSERT_TRUE(p2->isOnPrep(false));
    ASSERT_TRUE(p2->isOnPrep(true));

    // Set retention multiplier to 0, so will not be retained
    Parameters::instance()->putParameter(NETWORK_RETENTION_MULTIPLIER, 0.0);
    Parameters::instance()->putParameter(IS_NETWORK_DISRUPTION_ON, true);
    // only event should be the release event, so iterate schedule
    // and release person
    const repast::Schedule& schedule = repast::RepastProcess::instance()->getScheduleRunner().schedule();
    const_cast<repast::Schedule&>(schedule).execute();

    ASSERT_TRUE(net.contains(p2));
    ASSERT_FALSE(p2->isJailed());
    ASSERT_FALSE(net.hasEdge(p2, p1, 0));

    // Update multiplier to insure that edge is added back
    Parameters::instance()->putParameter(NETWORK_RETENTION_MULTIPLIER, 1000.0);
    net.addEdge(p4, p2, 0);

    jail.addPerson(schedule.getCurrentTick(), p2);
    const_cast<repast::Schedule&>(schedule).execute();
    ASSERT_TRUE(net.contains(p2));
    ASSERT_FALSE(p2->isJailed());
    ASSERT_TRUE(net.hasEdge(p4, p2, 0));

    // Set to 0 so shouldn't be added, but turn of disruption to override that
    Parameters::instance()->putParameter(NETWORK_RETENTION_MULTIPLIER, 0.0);
    Parameters::instance()->putParameter(IS_NETWORK_DISRUPTION_ON, false);

    jail.addPerson(schedule.getCurrentTick(), p2);
    ASSERT_FALSE(net.contains(p2));
    const_cast<repast::Schedule&>(schedule).execute();
    ASSERT_TRUE(net.contains(p2));
    ASSERT_FALSE(p2->isJailed());
    ASSERT_TRUE(net.hasEdge(p4, p2, 0));

    
}

TEST_F(JailTests, TestJailCareDistruption) {
    Diagnoser diagnoser(5, 0, 1);
    
    PersonPtr p1 = make_shared<Person>(1, 10, true, 0, 1, diagnoser);
    PersonPtr p2 = make_shared<Person>(2, 10, true, 0, 1, diagnoser);
    PersonPtr p3 = make_shared<Person>(3, 10, true, 0, 1, diagnoser);
    PersonPtr p4 = make_shared<Person>(4, 10, true, 0, 1, diagnoser);

    Network<Person> net;
    net.addVertex(p1);
    net.addVertex(p2);
    net.addVertex(p3);
    net.addVertex(p4);
    net.addEdge(p2, p1, 0);

    const repast::Schedule& schedule = repast::RepastProcess::instance()->getScheduleRunner().schedule();
    
    JailInfRateCalculator calc(0, 0, 0);
    Jail jail(&net, calc);
    p2->goOnPrep(1, 2);
    // Test care disruption
    Parameters::instance()->putParameter(IS_CARE_DISRUPTION_ON, true);
    p2->goOnART(schedule.getCurrentTick());
    ASSERT_TRUE(p2->isOnPrep(false));
    ASSERT_TRUE(p2->isOnPrep(true));
    ASSERT_TRUE(p2->isOnART(false));
    ASSERT_TRUE(p2->isOnART(true));
    jail.addPerson(schedule.getCurrentTick(), p2);
    ASSERT_TRUE(p2->isOnPrep(false));
    // Not on PreP when checking override, due to care disruption
    ASSERT_FALSE(p2->isOnPrep(true));

    // Release from jail
    const_cast<repast::Schedule&>(schedule).execute();
    ASSERT_TRUE(net.contains(p2));
    ASSERT_FALSE(p2->isJailed());
    ASSERT_TRUE(p2->isOnPrep(false));
    // Not on PreP when checking override, due to care disruption
    ASSERT_FALSE(p2->isOnPrep(true));
    ASSERT_TRUE(p2->isOnART(false));
    // Not on ART when checking override, due to care disruption
    ASSERT_FALSE(p2->isOnART(true));

    // run the next event, due to random seed and distribution draw, we know this is the
    // the ART end override
    const_cast<repast::Schedule&>(schedule).execute();
    ASSERT_TRUE(p2->isOnART(true));
    ASSERT_TRUE(p2->isOnART(false));
    ASSERT_FALSE(p2->isOnPrep(true));
    ASSERT_TRUE(p2->isOnPrep(false));

    // run the next event, due to random seed and distribution draw, we know this is the
    // the PreP end override
    const_cast<repast::Schedule&>(schedule).execute();
    ASSERT_TRUE(p2->isOnART(true));
    ASSERT_TRUE(p2->isOnART(false));
    ASSERT_TRUE(p2->isOnPrep(true));
    ASSERT_TRUE(p2->isOnPrep(false));
}

TEST_F(JailTests, TestMultiJailStayCareDisruption) {
    Diagnoser diagnoser(5, 0, 1);
    
    PersonPtr p1 = make_shared<Person>(1, 10, true, 0, 1, diagnoser);
    PersonPtr p2 = make_shared<Person>(2, 10, true, 0, 1, diagnoser);
    PersonPtr p3 = make_shared<Person>(3, 10, true, 0, 1, diagnoser);
    PersonPtr p4 = make_shared<Person>(4, 10, true, 0, 1, diagnoser);

    Network<Person> net;
    net.addVertex(p1);
    net.addVertex(p2);
    net.addVertex(p3);
    net.addVertex(p4);
    net.addEdge(p2, p1, 0);

    // With random seed of 1, jail schedule should look like:
    // Release 1  @ 33.1
    // Release 2  @ 189.2
    // Prep Off 1 @ 558.2
    // ART Off 1  @ 147.2
    // Prep Off 2 @ 189.3
    // ART Off 2  @ 201.3

    const repast::Schedule& schedule = repast::RepastProcess::instance()->getScheduleRunner().schedule();
    
    JailInfRateCalculator calc(0, 0, 0);
    Jail jail(&net, calc);
    p2->goOnPrep(1, 2);
    // Test care disruption
    Parameters::instance()->putParameter(IS_CARE_DISRUPTION_ON, true);
    p2->goOnART(1);
    ASSERT_TRUE(p2->isOnPrep(false));
    ASSERT_TRUE(p2->isOnPrep(true));
    ASSERT_TRUE(p2->isOnART(false));
    ASSERT_TRUE(p2->isOnART(true));
    jail.addPerson(1, p2);
    ASSERT_TRUE(p2->isOnPrep(false));
    // Not on PreP when checking override, due to care disruption
    ASSERT_FALSE(p2->isOnPrep(true));

    // Release from jail
    const_cast<repast::Schedule&>(schedule).execute();
    ASSERT_TRUE(net.contains(p2));
    ASSERT_FALSE(p2->isJailed());
    ASSERT_TRUE(p2->isOnPrep(false));
    // Not on PreP when checking override, due to care disruption
    ASSERT_FALSE(p2->isOnPrep(true));
    ASSERT_TRUE(p2->isOnART(false));
    // Not on ART when checking override, due to care disruption
    ASSERT_FALSE(p2->isOnART(true));

    // Go back in Jail before override period ends
    jail.addPerson(schedule.getCurrentTick(), p2);
    // back in jail so ART override is off
    ASSERT_TRUE(p2->isOnART(true));

    // run the next event -- ART Off 1 
    // person is in jail so state stays the same.
    const_cast<repast::Schedule&>(schedule).execute();
    // std::cout << "tick: " << schedule.getCurrentTick() << std::endl;
    ASSERT_TRUE(p2->isJailed());
    ASSERT_TRUE(p2->isOnART(true));
    ASSERT_TRUE(p2->isOnART(false));
    ASSERT_FALSE(p2->isOnPrep(true));
    ASSERT_TRUE(p2->isOnPrep(false));

    // run the next event, Release 2, 
    // so art / prep override is on
    const_cast<repast::Schedule&>(schedule).execute();
    // std::cout << "tick: " << schedule.getCurrentTick() << std::endl;
    ASSERT_FALSE(p2->isOnART(true));
    ASSERT_TRUE(p2->isOnART(false));
    ASSERT_FALSE(p2->isOnPrep(true));
    ASSERT_TRUE(p2->isOnPrep(false));

    // run the next event, prep off 2, should turn
    // prep override off
    const_cast<repast::Schedule&>(schedule).execute();
    // std::cout << "tick: " << schedule.getCurrentTick() << std::endl;
    ASSERT_FALSE(p2->isOnART(true));
    ASSERT_TRUE(p2->isOnART(false));
    ASSERT_TRUE(p2->isOnPrep(true));
    ASSERT_TRUE(p2->isOnPrep(false));

    // run the next event, ART off 2, should have effect
    const_cast<repast::Schedule&>(schedule).execute();
    // std::cout << "tick: " << schedule.getCurrentTick() << std::endl;
    ASSERT_TRUE(p2->isOnART(true));
    ASSERT_TRUE(p2->isOnART(false));
    ASSERT_TRUE(p2->isOnPrep(true));
    ASSERT_TRUE(p2->isOnPrep(false));

    // run the next event, prep off 1, but this should be
    // canceled by now
    const_cast<repast::Schedule&>(schedule).execute();
    // std::cout << "tick: " << schedule.getCurrentTick() << std::endl;
    ASSERT_TRUE(p2->isOnART(true));
    ASSERT_TRUE(p2->isOnART(false));
    ASSERT_TRUE(p2->isOnPrep(true));
    ASSERT_TRUE(p2->isOnPrep(false));
}

TEST_F(JailTests, TestInfRateCalc) {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, 20);
    std::vector<std::pair<int, int>> vals;
    for (int i = 0; i < 10; ++i) {
        int a = distribution(generator);
        int b = distribution(generator);

        if (a > b) {
            vals.push_back(std::make_pair(b, a));
        } else {
            vals.push_back(std::make_pair(a, b));
        }
    }

    JailInfRateCalculator calc(3, 0.5, 0.2);
    // return default rate because haven't met window yet.
    ASSERT_EQ(0.2, calc.calculateRate());
    for (size_t i = 0; i < vals.size(); ++i) {
        calc.addInfectionRate(vals[i].first, vals[i].second);
        double rate = calc.calculateRate();
        if (i < 2) {
            ASSERT_EQ(0.2, rate);
        } else {
            double sum = 0;
            int start = i - 2;
            for (int j = start; j < start + 3; ++j) {
                sum += (double)vals[j].first / vals[j].second;
            }
            ASSERT_EQ(sum / 3 * 0.5, rate);
        }
    }
}

TEST_F(JailTests, TestJailInfection) {

    Diagnoser diagnoser(5, 0, 1);
    PersonPtr p1 = make_shared<Person>(1, 10, true, 0, 1, diagnoser);
    PersonPtr p2 = make_shared<Person>(2, 10, true, 0, 1, diagnoser);
    Network<Person> net;
    net.addVertex(p1);
    net.addVertex(p2);
    net.addEdge(p2, p1);
    
    // unifected for 3 then infected
    JailInfRateCalculator calc(3, 1, 0);
    Jail jail(&net, calc);
    jail.addPerson(1, p1);
    for (int i = 0; i < 8; ++i) {
        // 100% inf rate
        jail.addOutsideInfectionRate(1, 1);
        std::vector<PersonPtr> infecteds;
        std::vector<EdgePtr<Person>> edges;
        jail.runInternalInfectionTransmission(i, infecteds, edges);
        if (i < 2) {
            ASSERT_TRUE(std::find(infecteds.begin(), infecteds.end(), p1) == infecteds.end());
            ASSERT_EQ(0, edges.size());
        } else {
            ASSERT_FALSE(std::find(infecteds.begin(), infecteds.end(), p1) == infecteds.end());
            ASSERT_EQ(1, edges.size());
            ASSERT_EQ(p2, edges[0]->v1());
            ASSERT_EQ(p1, edges[0]->v2());
        }
    }
}

TEST_F(JailTests, TestJailInmateAddRemove) {
    Diagnoser diagnoser(5, 0, 1);
    PersonPtr p1 = make_shared<Person>(1, 10, true, 0, 1, diagnoser);
    PersonPtr p2 = make_shared<Person>(2, 10, true, 0, 1, diagnoser);
    Network<Person> net;
    net.addVertex(p1);
    net.addVertex(p2);
    net.addEdge(p2, p1);
    
    // unifected for 3 then infected
    JailInfRateCalculator calc(3, 1, 0);
    Jail jail(&net, calc);
    jail.addPerson(1, p1);
    ASSERT_FALSE(net.contains(p1));
    ASSERT_FALSE(net.hasEdge(2, 1, 0));

    jail.addInmatesToNetwork();
    ASSERT_TRUE(net.contains(p1));
    ASSERT_TRUE(net.hasEdge(2, 1, 0));

    jail.removeInmatesFromNetwork();
    ASSERT_FALSE(net.contains(p1));
    ASSERT_FALSE(net.hasEdge(2, 1, 0));
}

TEST_F(JailTests, TestInitJailInfCalc) {
    Network<Person> net;
    
    JailInfRateCalculator calc(90, 0.5, 0.000009);
    Rcpp::List network = RInstance::rptr->parseEval("readRDS(\"../test_data/main_network_500.RDS\")");
    Rcpp::List gal = Rcpp::as<Rcpp::List>(network["gal"]);
    Rcpp::NumericVector rates = gal["external.infection.rates"];
    for (auto v : rates) {
        calc.addInfectionRate(v);
    }

    double expected_rate =  std::accumulate(rates.begin(), rates.end(), 0.0) / rates.size() * 0.5;
    ASSERT_EQ(expected_rate, calc.calculateRate());

    JailInfRateCalculator calc2(45, 0.5, 0.000009);
    for (auto v : rates) {
        calc2.addInfectionRate(v);
    }

    expected_rate =  std::accumulate(rates.begin() + 45, rates.end(), 0.0) / 45 * 0.5;
    ASSERT_EQ(expected_rate, calc2.calculateRate());

    JailInfRateCalculator calc3(100, 0.5, 0.000009);
    for (auto v : rates) {
        calc3.addInfectionRate(v);
    }

    // expect default because window is larger than saved data
    ASSERT_EQ(0.000009, calc3.calculateRate());
    

}

using namespace repast;

TEST_F(JailTests, TestJailedAndReleasedStatuses) {

    Diagnoser diagnoser(5, 0, 1);
    PersonPtr p1 = make_shared<Person>(1, 10, true, 0, 1, diagnoser);
    PersonPtr p2 = make_shared<Person>(2, 10, true, 0, 1, diagnoser);

    Network<Person> net(false);
    net.addVertex(p1);
    net.addVertex(p2);
    net.addEdge(p2, p1);
    const repast::Schedule& schedule = repast::RepastProcess::instance()->getScheduleRunner().schedule();

    Parameters::instance()->putParameter(PARTNER_WAS_JAILED_EXPIRATION_TIME, 2.0);
    Parameters::instance()->putParameter(RELEASED_PARTNER_EXPIRATION_TIME, 2.0);

    // unifected for 3 then infected
    JailInfRateCalculator calc(3, 1, 0);

    Jail jail(&net, calc);

    /* jailed expiration at 3.1 */
    jail.addPerson(1, p1);
    ASSERT_TRUE(p2->partnerWasJailed());
    ASSERT_FALSE(p2->hasReleasedPartner());

    /* released expiration at 3.1 */
    jail.releasePerson(2, p1);
    ASSERT_TRUE(p2->hasReleasedPartner());
    ASSERT_TRUE(p2->hasReleasedPartner(p1->id()));

    /* jailed expiration at 4.1 */
    jail.addPerson(3, p1);
    ASSERT_FALSE(p2->hasReleasedPartner());

    /* released expiration at 5.1 */
    jail.releasePerson(4, p1);
    ASSERT_TRUE(p2->hasReleasedPartner());
    ASSERT_TRUE(p2->partnerWasJailed());

    /* jailed expiration at 3.1; should not change jailed status because jailing before this happened */
    const_cast<repast::Schedule&>(schedule).execute();
    ASSERT_TRUE(p2->partnerWasJailed());

    /* released expiration at 3.1 should not change released status */
    const_cast<repast::Schedule&>(schedule).execute();
    ASSERT_TRUE(p2->hasReleasedPartner());

    //const_cast<Schedule&>(schedule).schedule_event(3, mf);
    /* jailed expiration at 4.1 */
    const_cast<repast::Schedule&>(schedule).execute();
    ASSERT_TRUE(p2->hasReleasedPartner());
    ASSERT_FALSE(p2->partnerWasJailed());

    /* released expiration at 5.1 */
    const_cast<repast::Schedule&>(schedule).execute();
    ASSERT_FALSE(p2->hasReleasedPartner());
}



