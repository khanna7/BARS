#include <memory>
#include <vector>
#include <iostream>

#include "repast_hpc/initialize_random.h"
#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/io.h"

#include "Model.h"
#include "Parameters.h"
#include "utils.h"
#include "file_utils.h"
#include "FileOutput.h"

#define PROFILING

#ifdef PROFILING
    #include "boost/timer/timer.hpp"
#endif

using namespace Rcpp;
using namespace TransModel;

void init_network(std::shared_ptr<RInside>& R, const std::string& r_file, const std::string& init_data_file) {
    std::string cmd = "source(file=\"" + r_file + "\")";
    R->parseEvalQ(cmd);
    as<Function>((*R)["trans_model_init"])(init_data_file);
}

void load_networks(std::shared_ptr<RInside>& R, const std::string& main_file, const std::string& casual_file) {
    std::string cmd = "source(file=\"../r/network_model/load_serialized_networks.R\")";
    R->parseEvalQ(cmd);
    as<Function>((*R)["load.networks"])(main_file, casual_file);
}

void usage() {
    std::cerr << "usage: transmission_model model_config [parameter string]" << std::endl;
    std::cerr << "  model_config: path to the model properties file" << std::endl;
    std::cerr
            << "  parameter_string: optional string of parameters to override those in the non derived R parameters file"
            << std::endl;
}

void run(std::string propsFile, int argc, char** argv) {
    // pause for selecting process in profiler
    // std::cout << "enter to continue" << std::endl;
    // getchar();

    //boost::mpi::communicator comm;
    //if (comm.rank() == 0) {
    std::string time;
    repast::timestamp(time);
    std::cout << "Start Time: " << time << std::endl;
    //}

#ifdef PROFILING
    boost::timer::cpu_timer timer;
    timer.start();
#endif

    repast::Properties props(propsFile, argc, argv);
    std::shared_ptr<RInside> R = std::make_shared<RInside>(argc, argv);

    TransModel::Parameters::initialize(props);
    const std::string non_derived(Parameters::instance()->getStringParameter(R_PARAMETERS_NON_DERIVED));
    const std::string derived(Parameters::instance()->getStringParameter(R_PARAMETERS_DERIVED));

    std::string param_string;
    if (argc == 3) {
        param_string = argv[2];
    }

    TransModel::init_parameters(non_derived, derived, param_string, Parameters::instance(), R);
    FileOutput out(unique_file_name(output_directory(Parameters::instance()) + "/parameters.txt"));
    out.ostream() << Parameters::instance();
    out.close();

    props.putProperty("global.random.seed", Parameters::instance()->getIntParameter("global.random.seed"));
    repast::initializeRandom(props);

    init_network(R, TransModel::Parameters::instance()->getStringParameter(R_FILE), 
        TransModel::Parameters::instance()->getStringParameter(INIT_DATA_FILE));

    std::string net_var = Parameters::instance()->getStringParameter(NET_VAR);
    std::string cas_net_var = Parameters::instance()->getStringParameter(CASUAL_NET_VAR);

    if (Parameters::instance()->contains(MAIN_NETWORK_FILE)) {
        std::string main_file = Parameters::instance()->getStringParameter(MAIN_NETWORK_FILE);
        std::string casual_file = Parameters::instance()->getStringParameter(CASUAL_NETWORK_FILE);
        load_networks(R, main_file, casual_file);
    }

    // constructor should schedule the step method
    TransModel::Model model(R, net_var, cas_net_var);


#ifdef PROFILING
    timer.stop();
    std::cout << "Initialization Time: " << timer.format(6, "%t") << std::endl;
#endif

#ifdef PROFILING
    timer.start();
#endif
    repast::RepastProcess::instance()->getScheduleRunner().run();

#ifdef PROFILING
    timer.stop();
    std::cout << "Run Time: " << timer.format(6, "%t") << std::endl;
#endif

    //if (comm.rank() == 0) {
    //std::string time;
    repast::timestamp(time);
    std::cout << "End Time: " << time << std::endl;
    //}
}

int main(int argc, char *argv[]) {
    boost::mpi::environment env(argc, argv);

    if (argc < 2 || argc > 3) {
        usage();
        return -1;
    }

    std::string props(argv[1]);

    try {
        //boost::mpi::communicator world;
        repast::RepastProcess::init("");
        run(props, argc, argv);
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
    }
    repast::RepastProcess::instance()->done();

    return 0;
}
