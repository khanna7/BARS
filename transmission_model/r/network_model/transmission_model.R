rm(list=ls())

suppressMessages(library(ergm))
suppressMessages(library(tergm))
suppressMessages(library(network))
suppressMessages(library(networkDynamic))

load(file="../r/network_model/initialized-model.RData")

nw <- fit$network
formation <- fit$formula
dissolution <- ~offset(edges)

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
                   #time.start = ts,
                   time.start = time,
                   #start.time = time,
                   time.slices = 1#,
                   #monitor = stats.form,
                   #control = control.simulate.network(MCMC.burnin=10000)
    )
    changes
    
    
    #print(network.size(nw))
    #print("sim end")
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


