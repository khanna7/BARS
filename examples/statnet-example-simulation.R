## example using statnet: dynamic simulation code
rm(list=ls())

library(ergm)
library(tergm)
library(network)
library(networkDynamic)

source(file="update-vital-dynamics.R")
load(file="example-estimation.RData")


nw <- fit$network
formation <- fit$formula
dissolution <- ~offset(edges)
popsize <- network.size(nw)

timesteps <- 25
start.time <- 1

## Start with all vertices and edges active
nw <- activate.vertices(nw,onset=1,terminus=Inf)
nw <- activate.edges(nw,onset=1,terminus=Inf)

for (time in 2:timesteps) {
  
  set.seed(Sys.time())
  
  cat("Completing time step", time,
      "of", timesteps, "\n")
  
  nw <- simulate(nw,
                 formation = formation, 
                 dissolution = dissolution,
                 coef.form = theta.form, 
                 coef.diss = theta.diss,
                 constraints = constraints,
                 #time.start = ts,
                 time.start = time,
                 #start.time = time,
                 time.slices = 1#,
                 #monitor = stats.form,
                 #control = control.simulate.network(MCMC.burnin=10000)
  )
  
  #############################################################
  nw #networkDynamic object
  
  ## Nodal attributes and edgelist on cross-sectional network
  xn.net <- network.collapse(nw, at=time) 
             #obtain cross-sectional network slice
  xn.net #info on cross-sectional network slice
  xn.vertex.att.list <- list.vertex.attributes(xn.net) 
           #vertex attributes list
  xn.att <- lapply(xn.vertex.att.list, function (x) xn.net%v%x) 
           # values of each vertex attribute
  xn.el <- as.edgelist(xn.net)
           #edgelist of cross-sectional network
  
  ## Nodal attributes and edgelist on full network
  nw.el <- as.edgelist(nw) 
  nw.vertex.att.list <- list.vertex.attributes(nw)
  nw.att <- lapply(nw.vertex.att.list, function (x) nw%v%x)
  #############################################################
  
  nw <- update.vital.dynamics(nw,
                              max.survival=50,
                              daily.death.prob=0.01/100,
                              daily.birth.rate=1/100
                              )
  cat("Total number of edges is ",
      network.edgecount(nw), "\n")
  cat("Number of alive edges is ",
      network.edgecount(network.extract(nw, at=time)),
      "\n\n") 
  
}