/*
 * CondomUseAssigner.h
 *
 *  Created on: Jan 12, 2017
 *      Author: nick
 */

#ifndef SRC_CONDOMUSEASSIGNER_H_
#define SRC_CONDOMUSEASSIGNER_H_

#include <memory>
#include <vector>

#include "Edge.h"
#include "common.h"

namespace TransModel {

enum class PartnershipType { SERODISCORDANT, SEROCONCORDANT };

struct CondomUseProbabilities {
    double category_probabilty, use_probabilty;
};


class CondomUseAssigner {

private:
    friend class CondomUseAssignerFactory;
    std::vector<CondomUseProbabilities> casual_sd_probs, casual_sc_probs;
    std::vector<CondomUseProbabilities> steady_sd_probs, steady_sc_probs;

    void updateEdge(std::vector<CondomUseProbabilities>& vec, std::shared_ptr<Edge<Person>> edge);

public:
    CondomUseAssigner();
    virtual ~CondomUseAssigner();


    virtual void initEdge(std::shared_ptr<Edge<Person>> edge);
};


class CondomUseAssignerFactory {

private:
    std::vector<CondomUseProbabilities> casual_sd_probs, casual_sc_probs;
    std::vector<CondomUseProbabilities> steady_sd_probs, steady_sc_probs;

    void updateVector(std::vector<CondomUseProbabilities>& vec, double category_probability, double use_probabilty);
    void checkVector(std::vector<CondomUseProbabilities>& vec);

public:
    CondomUseAssignerFactory();
    virtual ~CondomUseAssignerFactory();

    CondomUseAssigner createAssigner();
    void addProbability(PartnershipType type, int network_type, double category_probability, double use_probabilty);

};

} /* namespace TransModel */

#endif /* SRC_CONDOMUSEASSIGNER_H_ */
