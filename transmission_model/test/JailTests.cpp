#include "gtest/gtest.h"


#include "repast_hpc/RepastProcess.h"

#include "Jail.h"
#include "Parameters.h"
#include "PersonCreator.h"
#include "StatsBuilder.h"
#include "utils.h"

using namespace TransModel;
using namespace std;


class JailTests: public ::testing::Test {

protected:

    JailTests() {

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

    Network<Person> net(false);
    net.addVertex(p1);
    net.addVertex(p2);
    net.addVertex(p3);
    net.addVertex(p4);
    net.addEdge(p2, p1, 0);
    

    Jail jail(&net);
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

    Network<Person> net(false);
    net.addVertex(p1);
    net.addVertex(p2);
    net.addVertex(p3);
    net.addVertex(p4);
    net.addEdge(p2, p1, 0);

    const repast::Schedule& schedule = repast::RepastProcess::instance()->getScheduleRunner().schedule();
    
    Jail jail(&net);
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
    ASSERT_TRUE(p2->isOnART(false));

    // run the next event, due to random seed and distribution draw, we know this is the
    // the PreP end override
    const_cast<repast::Schedule&>(schedule).execute();
    ASSERT_TRUE(p2->isOnART(true));
    ASSERT_TRUE(p2->isOnART(false));
    ASSERT_TRUE(p2->isOnPrep(true));
    ASSERT_TRUE(p2->isOnART(false));
}