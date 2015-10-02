
#include <RInside.h>

extern "C" {
#include "access.h"
}

using namespace Rcpp;

template <typename T>
T get_vertex_attribute(List nw, int vertex_idx, std::string attribute) {
    List v = as<List>(as<List>(nw["val"])[vertex_idx]);
    return v[attribute];
}

int main(int argc, char *argv[]) {
    
    try {

        RInside R(argc, argv);
        // std::string txt = "source(file=\"./statnet-example.R\")";
        std::string txt = "load(file=\"./statnet-example.RData\")";
        R.parseEvalQ(txt);
        
        List nw = as<List>(R["nw"]);
        List gal = as<List>(nw["gal"]);
        double n = gal["n"];
        std::cout << "Vertex Count: " << n << std::endl;
        
        int age = get_vertex_attribute<int>(nw, 250, "age");
        std::cout << "Age: " << age << std::endl;
        
        std::cout << isAdjacent(nw, 102, 305, 0) << std::endl;
        
        
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
    }

  exit(0);
}
