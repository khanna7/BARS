rm(list=ls())



suppressMessages(library(ergm))

suppressMessages(library(tergm))

suppressMessages(library(network))

suppressMessages(library(networkDynamic))


load(file="../r/network_model/cas_net.RData")

formation <- fit$formula

dissolution <- dissolution

nw_simulate <-

  function(net, time) {

    #print("sim start")

    

    #print(formation)

    #print(dissolution)

    #print(theta.form)

    #print(theta.diss)

    #print(constraints)

    

    class(net) <- "network"

    changes <- simulate(net,

                   formation = formation, 

                   dissolution = dissolution,

                   coef.form = theta.form, 

                   coef.diss = theta.diss,

                   constraints = constraints,

                   output = "changes",

                   time.start = time,

                   time.slices = 1

    )

    changes

    

    

    #print(network.size(nw))

    #print("sim end")

  }

n_cas <- cas_fit$network

cas_formation <- cas_fit$formula

cas_dissolution <- dissolution_cas


n_cas_simulate <-

  function(cas_net, time) {

    class(cas_net) <- "network"

    changes <- simulate(cas_net,

                        formation = cas_formation, 

                        dissolution = cas_dissolution,

                        coef.form = theta.form_cas, 

                        coef.diss = theta.diss_cas,

                        constraints = constraints_cas,

                        output = "changes",

                        time.start = time,

                        time.slices = 1

    )

    changes

  }
  

nw_save <- function(net, fname) {

	class(net) <- "network"

	saveRDS(net, file=fname)

}



# holds any code that I want to call from 

# C++ to make sure changes there are being

# reflected here.

nw_debug <- function() {

  #print(nw.active$val[[1]]$age)

  print(nw$val[[1]]$age)

  #nw.active <<- network.extract(nw,at = 2, retain.all.vertices = T)

  age <- nw$val[[1]]$age

  nw <<- set.vertex.attribute(nw, "age", age + 1, v=c(1))

  #print(nw.active$val[[1]]$age)

} 





