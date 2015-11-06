
#include <memory>
#include <vector>

#include "Parameters.h"
#include "repast_hpc/initialize_random.h"

#include "Model.h"

#include "RInside.h"

using namespace Rcpp;

void init_network(std::shared_ptr<RInside>& R, const std::string& r_file) {
	std::string cmd = "source(file=\"" + r_file + "\")";
	R->parseEvalQ(cmd);
}

int main(int argc, char *argv[]) {
    
    try {
        std::shared_ptr<RInside> R = std::make_shared<RInside>(argc, argv);
        repast::Properties props(argv[1]);
        repast::initializeRandom(props);
        TransModel::Parameters::initialize(props);
        init_network(R, TransModel::Parameters::instance()->getStringParameter(TransModel::R_FILE));

        TransModel::Model model(R, "nw");
        model.run();
        
 //       List nw = as<List>((*R)["nw"]);
//        List mel = as<List>(nw["mel"]);
//
//        TransModel::IsActiveEdge predicate;
       // TransModel::EdgeIterator iterBegin(mel.begin(), TransModel::MELToEdgePair());
        //TransModel::EdgeIterator iterEnd(mel.end(), TransModel::MELToEdgePair());
        //TransModel::EdgeFilterIter iterBegin(predicate, mel.begin(), mel.end());
        //TransModel::EdgeFilterIter iterEnd(predicate, mel.end(), mel.end());
//
//        int count = 0;
//        for (List::iterator iter = mel.begin(); iter != mel.end(); ++iter) {
//        	SEXP item = (*iter);
 //       }
//

        //for (TransModel::EdgeFilterIter iter = iterBegin; iter != iterEnd; ++iter) {
        //	++count;
        //}
       // std::cout << count << std::endl;

//        for (auto& exp : mel) {
//        	List item = as<List>(exp);
//        	std::cout << TYPEOF(item["outl"]) << std::endl;
//        	std::cout << (as<IntegerVector>(item["outl"]))[0] << " -> " << (as<IntegerVector>(item["inl"]))[0] << std::endl;
//        	List atl = as<List>(item["atl"]);
//        	std::cout << TYPEOF(atl["na"]) << std::endl;
//        	std::cout << (as<LogicalVector>(atl["na"]))[0] << ", " << std::endl;
//        	break;
//        }


//        std::cout << get_vertex_attribute<int>(nw, 0, "age") << std::endl;
//        set_vertex_attribute<int>(nw, 0, "age", 104);
//        Function extract = (*R)["nw_extract"];
//		for (int i = 0; i < 2; ++i) {
//			extract();
//			//
//			nw = as<List>((*R)["nw"]);
//			std::cout << get_vertex_attribute<int>(nw, 0, "age") << std::endl;
//		}

//        Function simulate = R["nw_simulate"];
//        for (int t = 2; t < 25; ++t) {
//        	std::cout << "t: " << t << " simulating" << std::endl;
//        	R["time"] = t;
//        	simulate();
//        }
        
//        List nw = as<List>(R["nw"]);
//        List gal = as<List>(nw["gal"]);
//        double n = gal["n"];
//        std::cout << "Vertex Count: " << n << std::endl;
//
//        int age = get_vertex_attribute<int>(nw, 250, "age");
//        std::cout << "Age: " << age << std::endl;
//
//        std::cout << isAdjacent(nw, 102, 305, 0) << std::endl;
        
        
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
    }

  exit(0);
}
