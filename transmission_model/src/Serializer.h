#ifndef SRC_SERIALIZER_H
#define SRC_SERIALIZER_H

#include "repast_hpc/Schedule.h"

#include "Network.h"
#include "Jail.h"
#include "Person.h"
#include "RInside.h"

namespace TransModel {

class Serializer : public repast::Functor {

private:
    Network<Person>* net_;
    Jail* jail_;
    std::shared_ptr<RInside> R_;

 public:
    Serializer(Network<Person>* net, Jail* jail, std::shared_ptr<RInside> R);
    ~Serializer() {}
    void operator()();
};

}


#endif