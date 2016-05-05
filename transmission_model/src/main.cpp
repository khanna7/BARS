#include <memory>
#include <vector>

#include "repast_hpc/initialize_random.h"
#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/io.h"

#include "Model.h"
#include "Parameters.h"
#include "utils.h"

using namespace Rcpp;
using namespace TransModel;

void init_network(std::shared_ptr<RInside>& R, const std::string& r_file) {
	std::string cmd = "source(file=\"" + r_file + "\")";
	R->parseEvalQ(cmd);
}

void usage() {
	std::cerr << "usage: transmission_model repast_config model_config" << std::endl;
	std::cerr << "  first string: string is the path to the Repast HPC \n\tconfiguration properties file" << std::endl;
	std::cerr << "  second string: string is the path to the model properties file" << std::endl;
}

void run(std::string propsFile, int argc, char** argv) {
	boost::mpi::communicator comm;
	if (comm.rank() == 0) {
		std::string time;
		repast::timestamp(time);
		std::cout << "Start Time: " << time << std::endl;
	}

	repast::Properties props(propsFile, argc, argv);
	//props.putProperty("random.seed", i);
	repast::initializeRandom(props);
	std::shared_ptr<RInside> R = std::make_shared<RInside>(argc, argv);

	TransModel::Parameters::initialize(props);
	TransModel::add_from_R(Parameters::instance()->getStringParameter(R_PARAMETERS_FILE), Parameters::instance(), R);
	init_network(R, TransModel::Parameters::instance()->getStringParameter(R_FILE));

	std::string net_var = Parameters::instance()->getStringParameter(NET_VAR);
	std::string cas_net_var = Parameters::instance()->getStringParameter(CASUAL_NET_VAR);

	// constructor should schedule the step method
	TransModel::Model model(R, "nw", cas_net_var);
	// now can run
	repast::RepastProcess::instance()->getScheduleRunner().run();

	if (comm.rank() == 0) {
		std::string time;
		repast::timestamp(time);
		std::cout << "End Time: " << time << std::endl;
	}
}

int main(int argc, char *argv[]) {
	boost::mpi::environment env(argc, argv);

	if (argc < 3) {
		usage();
		return -1;
	}

	std::string config(argv[1]);
	std::string props(argv[2]);

	try {
		boost::mpi::communicator world;
		repast::RepastProcess::init(config);
		run(props, argc, argv);
	} catch (std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	} catch (...) {
		std::cerr << "Unknown exception caught" << std::endl;
	}
	repast::RepastProcess::instance()->done();

	return 0;
}
