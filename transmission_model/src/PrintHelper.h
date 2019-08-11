/*
 * PrintHelper.h
 *
 *  Created on: Aug 1, 2019
 *      Author: Babak
 */

#ifndef SRC_PRINTHELPER_H_
#define SRC_PRINTHELPER_H_

using namespace std;

namespace TransModel {

struct PrintHelper {

    static void printPersonNetwork(PersonPtr person, Network<Person>* net) {
        std::cout << "network for person id: " << person->id() << std::endl;
        std::vector<EdgePtr<Person>> edges;
        net->getEdges(person, edges);
        for (auto edge : edges) {
            std::cout << "[i" << edge->id() << ",t" << edge->type() << "(" << edge->v1()->id()<< "," << edge->v2()->id() << ")]" << std::endl;
        }
    }
};


} /* namespace TransModel */

#endif /* SRC_PRINTHELPER_H_ */