#ifndef SRC_PERSONTOVAL_H
#define SRC_PERSONTOVAL_H

#include "common.h"

#include "Rcpp.h"

namespace TransModel {

struct PersonToVAL {

    Rcpp::List operator()(const PersonPtr& p, int idx, double tick) const;

};

}

#endif