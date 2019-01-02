/*
 * debug_utils.cpp
 *
 *  Created on: Jan 13, 2017
 *      Author: nick
 */

#include "debug_utils.h"

#include "FileOutput.h"

namespace TransModel {

void write_edges(Network<Person>& net, const std::string fname) {
        FileOutput out(fname);

        out << "type,discordant,use_prob" << "\n";

        for (auto iter = net.edgesBegin(); iter != net.edgesEnd(); ++iter) {
            auto edge = *iter;
            bool discordant = (edge->v1()->isInfected() && !edge->v2()->isInfected()) || (!edge->v1()->isInfected() && edge->v2()->isInfected());
            out << edge->type() << "," << discordant << "," << edge->condomUseProbability() << "\n";
        }

        out.close();
    }
}


