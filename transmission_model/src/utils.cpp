/*
 * utils.cpp
 *
 *  Created on: Feb 16, 2016
 *      Author: nick
 */
#include <exception>
#include <map>

#include "repast_hpc/Random.h"
#include "utils.h"

#include "boost/tokenizer.hpp"
#include "boost/algorithm/string.hpp"

//#include "boost/algorithm/string.hpp"

using namespace std;

namespace TransModel {

const std::string ACUTE_LENGTH = "acute.length";
const std::string CHRONIC_LENGTH = "chronic.length";
const std::string LATE_LENGTH = "late.length";

void parse_parameters(std::map<string, double>& props, std::map<string, string>& string_props,
                const std::string& param_string) {

    boost::char_separator<char> comma_sep(",");
    boost::tokenizer<boost::char_separator<char> > comma_tok(param_string, comma_sep);

    for (auto item : comma_tok) {
        boost::trim(item);
        size_t pos = item.find_first_of("=");
        if (pos == std::string::npos) {
            throw invalid_argument("Invalid parameter: " + item);
        }

        string key(item.substr(0, pos));
        boost::trim(key);
        if (key.length() == 0) {
            throw invalid_argument("Invalid parameter: " + item);
        }

        string val(item.substr(pos + 1, item.length()));
        boost::trim(val);
        if (val.length() == 0) {
            throw invalid_argument("Invalid parameter: " + item);
        }
        try {
            //std::cout << key << ": " << val << std::endl;
            if (val.find("|") != std::string::npos) {
                string_props.emplace(key, val);
            } else {
                props[key] = stod(val);
            }
        } catch (...) {
            string_props.emplace(key, val);
        }
    }
}

void param_string_to_R_vars(const std::string& param_string, Parameters* params, std::shared_ptr<RInside> R) {
    map<string, double> props;
    map<string, string> string_props;
    parse_parameters(props, string_props, param_string);
    Rcpp::Environment env;
    for (auto item : props) {
        // check if variable exists in env. If not,
        // then might be derived param being overriden
        if (!env.exists(item.first)) {
            throw std::invalid_argument("WARNING: " + item.first + " is not a non-derived parameter but is being set from the command line.");
        }
        (*R)[item.first] = item.second;
    }

    for (auto item : string_props) {
        //std::cout << item.first << ": " << item.second << std::endl;
        if (!env.exists(item.first)) {
            throw std::invalid_argument("WARNING: " + item.first + " is not a non-derived parameter but is being set from the command line.");
        }
        (*R)[item.first] = item.second;
        //params->putParameter(item.first, item.second);
    }
}

void init_parameters(const std::string& non_derived, const std::string& derived, const std::string& param_string,
                Parameters* params, std::shared_ptr<RInside> R) {

    std::string cmd = "source(file=\"" + non_derived + "\")";
    R->parseEvalQ(cmd);

    param_string_to_R_vars(param_string, params, R);

    //std::cout << (Rcpp::as<Rcpp::NumericVector>((*R)["prep.bl.use.prop.gte"]))[0] << std::endl;

    cmd = "source(file=\"" + derived + "\")";
    R->parseEvalQ(cmd);

    cmd = "set.seed(" + std::to_string(repast::Random::instance()->seed()) + ")";
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
        //std::cout << name << ": " << TYPEOF(val) <<  std::endl;
        if (TYPEOF(val) == REALSXP) {
            Rcpp::NumericVector v = Rcpp::as<Rcpp::NumericVector>(val);
            if (v.size() == 1) {
                std::ostringstream out;
                out << std::setprecision(10) << v[0];
                params->putParameter(name, out.str());

            } else if (v.size() > 1) {
                SEXP r;
                R->parseEval("paste(" + name + ")", r);
                Rcpp::CharacterVector cv = Rcpp::as<Rcpp::CharacterVector>(r);
                std::string s;
                for (int i = 0; i < cv.size(); ++i) {
                    if (i > 0)
                        s += ",";
                    s += Rcpp::as<string>(cv[i]);
                }
                params->putParameter(name, s);
            }
            //std::cout << "extern const std::string " << upper_name << ";" << std::endl; //" = \"" << name << "\";" << std::endl;
        } else if (TYPEOF(val) == INTSXP && (name == ACUTE_LENGTH || name == CHRONIC_LENGTH || name == LATE_LENGTH)) {
            Rcpp::IntegerVector vec = Rcpp::as<Rcpp::IntegerVector>(val);
            params->putParameter(name + ".min", (double) vec[0]);
            params->putParameter(name + ".max", (double) vec[vec.size() - 1]);
            //std::cout << "extern const std::string " << upper_name << "_MIN;" << std::endl; //<< " = \"" << name << ".min" << "\";" << std::endl;
            //std::cout << "extern const std::string " << upper_name << "_MAX;" << std::endl; //<< " = \"" << name << ".max" << "\";" << std::endl;
        } else if (TYPEOF(val) == INTSXP) {
            Rcpp::IntegerVector vec = Rcpp::as<Rcpp::IntegerVector>(val);
            if (vec.size() == 1) {
                params->putParameter(name, std::to_string(vec[0]));
            } else {
                throw std::invalid_argument("Parameter error: " + name + " int vectors not supported");
            }
        } else if (TYPEOF(val) == STRSXP) {
            params->putParameter(name, Rcpp::as<std::string>(val));
        }
    }

    //std::cout << Parameters::instance()->getDoubleParameter("prep.bl.use.prop.gte") << std::endl;
}

std::string output_directory(Parameters* params) {
    std::string out_dir = params->getStringParameter(OUTPUT_DIR);
    if (params->contains(RUN_NUMBER)) {
        out_dir += "_" + std::to_string(params->getIntParameter(RUN_NUMBER));
    }
    return out_dir;
}

}
