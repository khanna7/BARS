
#include <RInside.h>

extern "C" {
#include "access.h"
}

using namespace Rcpp;

template <typename T>
T get_vertex_attribute(List& nw, int vertex_idx, std::string attribute) {
    List v = as<List>(as<List>(nw["val"])[vertex_idx]);
    return v[attribute];
}

void init_network(RInside& R, char* argv[]) {
	std::string file(argv[1]);
	std::string cmd = "source(file=\"" + file + "\")";
	R.parseEvalQ(cmd);
}

int main(int argc, char *argv[]) {
    
    try {

        RInside R(argc, argv);
        init_network(R, argv);
        
        Function simulate = R["nw_simulate"];
        for (int t = 2; t < 25; ++t) {
        	std::cout << "t: " << t << " simulating" << std::endl;
        	R["time"] = t;
        	simulate();
        }
        
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
