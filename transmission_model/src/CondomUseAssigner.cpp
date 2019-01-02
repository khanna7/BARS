/*
 * CondomUseAssigner.cpp
 *
 *  Created on: Jan 12, 2017
 *      Author: nick
 */

#include <cmath>

#include "CondomUseAssigner.h"
#include "Person.h"

#include "repast_hpc/Random.h"

namespace TransModel {

CondomUseAssigner::CondomUseAssigner() : casual_sd_probs{}, casual_sc_probs{}, steady_sd_probs{},
    steady_sc_probs{} {
}

CondomUseAssigner::~CondomUseAssigner() {
}

void CondomUseAssigner::updateEdge(std::vector<CondomUseProbabilities>& vec, std::shared_ptr<Edge<Person>> edge) {
    double draw = repast::Random::instance()->nextDouble();
    for (auto& probs : vec) {
        if (draw <= probs.category_probabilty) {
            edge->setCondomUseProbability(probs.use_probabilty);
            break;
        }
    }
}

void CondomUseAssigner::initEdge(std::shared_ptr<Edge<Person>> edge) {
    PersonPtr p1 = edge->v1();
    PersonPtr p2 = edge->v2();
    bool discordant = (p1->isInfected() && !p2->isInfected()) || (!p1->isInfected() && p2->isInfected());
    if (edge->type() == STEADY_NETWORK_TYPE) {
        if (discordant) {
            updateEdge(steady_sd_probs, edge);
        } else {
            updateEdge(steady_sc_probs, edge);
        }
    } else if (edge->type() == CASUAL_NETWORK_TYPE) {
        if (discordant) {
            updateEdge(casual_sd_probs, edge);
        } else {
            updateEdge(casual_sc_probs, edge);
        }
    } else {
        throw std::invalid_argument("Invalid edge type in CondomUseAssigner::assign");
    }

}


CondomUseAssignerFactory::CondomUseAssignerFactory() : casual_sd_probs{}, casual_sc_probs{}, steady_sd_probs{},
        steady_sc_probs{} {}
CondomUseAssignerFactory:: ~CondomUseAssignerFactory() {}

CondomUseAssigner CondomUseAssignerFactory::createAssigner() {
    checkVector(steady_sd_probs);
    checkVector(steady_sc_probs);
    checkVector(casual_sd_probs);
    checkVector(casual_sc_probs);

    CondomUseAssigner assigner;
    assigner.steady_sd_probs.insert(assigner.steady_sd_probs.end(), steady_sd_probs.begin(), steady_sd_probs.end());
    assigner.steady_sc_probs.insert(assigner.steady_sc_probs.end(), steady_sc_probs.begin(), steady_sc_probs.end());
    assigner.casual_sd_probs.insert(assigner.casual_sd_probs.end(), casual_sd_probs.begin(), casual_sd_probs.end());
    assigner.casual_sc_probs.insert(assigner.casual_sc_probs.end(), casual_sc_probs.begin(), casual_sc_probs.end());

    return assigner;
}

bool within(double a, double b, double epsilon) {
    return fabs(a - b) <= epsilon;
}

void CondomUseAssignerFactory::checkVector(std::vector<CondomUseProbabilities>& vec) {
    // check if final val is near enough to 1 given some rounding errors,
    // if so make it 1.0 exactly.
    if (vec.size() == 0) {
        throw std::invalid_argument("Probability vector is empty in CondomUseAssignerFactory::checkVector");
    } else if (! within(vec[vec.size() - 1].category_probabilty, 1.0, 0.001)) {
        throw std::invalid_argument("Final category_probability != 1.0 in CondomUseAssignerFactory::checkVector: " + std::to_string(vec[vec.size() - 1].category_probabilty));
    }

    vec[vec.size() - 1].category_probabilty = 1.0;
}

void CondomUseAssignerFactory::updateVector(std::vector<CondomUseProbabilities>& vec, double category_probability, double use_probability) {
    if (vec.size() == 0) {
        vec.push_back({category_probability, use_probability});
    } else {
        vec.push_back({vec[vec.size() - 1].category_probabilty + category_probability, use_probability});
    }
}

void CondomUseAssignerFactory::addProbability(PartnershipType type, int network_type, double category_probability, double use_probabilty) {
    if (network_type == STEADY_NETWORK_TYPE) {
        if (type == PartnershipType::SERODISCORDANT) {
            updateVector(steady_sd_probs, category_probability, use_probabilty);
        } else {
            updateVector(steady_sc_probs, category_probability, use_probabilty);
        }
    } else if (network_type == CASUAL_NETWORK_TYPE) {
        if (type == PartnershipType::SERODISCORDANT) {
            updateVector(casual_sd_probs, category_probability, use_probabilty);
        } else {
            updateVector(casual_sc_probs, category_probability, use_probabilty);
        }
    } else {
        throw std::invalid_argument("Invalid edge type in CondomUseAssigner::assign");
    }
}

} /* namespace TransModel */
