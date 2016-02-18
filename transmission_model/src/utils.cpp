/*
 * utils.cpp
 *
 *  Created on: Feb 16, 2016
 *      Author: nick
 */

#include "utils.h"

//#include "boost/algorithm/string.hpp"

namespace TransModel {

const std::string ACUTE_LENGTH = "acute.length";
const std::string CHRONIC_LENGTH = "chronic.length";
const std::string LATE_LENGTH = "late.length";

void add_from_R(const std::string& r_file, Parameters* params, std::shared_ptr<RInside> R) {

	std::string cmd = "source(file=\"" + r_file + "\")";
	R->parseEvalQ(cmd);
	SEXP result;
	R->parseEval("ls()", result);

	Rcpp::List list = Rcpp::as<Rcpp::List>(result);
	for (auto& v : list) {
		std::string name(Rcpp::as<std::string>(v));
		//std::string upper_name(Rcpp::as<std::string>(v));
		//boost::to_upper(upper_name);
		//boost::replace_all(upper_name, ".", "_");
		SEXP val((*R)[name]);
		if (TYPEOF(val) == REALSXP) {
			params->putParameter(name, Rcpp::as<double>(val));
			//std::cout << "extern const std::string " << upper_name << ";" << std::endl; //" = \"" << name << "\";" << std::endl;
		} else if (TYPEOF(val) == INTSXP && (name == ACUTE_LENGTH || name == CHRONIC_LENGTH ||
				name == LATE_LENGTH)) {
			Rcpp::IntegerVector vec = Rcpp::as<Rcpp::IntegerVector>(val);
			params->putParameter(name + ".min", (double)vec[0]);
			params->putParameter(name + ".max", (double)vec[vec.size() - 1]);
			//std::cout << "extern const std::string " << upper_name << "_MIN;" << std::endl; //<< " = \"" << name << ".min" << "\";" << std::endl;
			//std::cout << "extern const std::string " << upper_name << "_MAX;" << std::endl; //<< " = \"" << name << ".max" << "\";" << std::endl;
		}
	}
}

}

