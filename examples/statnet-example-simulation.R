## example using statnet: dynamic simulation code
rm(list=ls())

library(ergm)
library(tergm)
library(network)
library(networkDynamic)

source(file="update-vital-dynamics.R")
source(file="model-transmission.R")
load(file="example-estimation.RData")

collect.stats <- function(time) {
  xn.nw <- network.collapse(nw, at=time)
  xn.infected <- which(xn.nw%v%"inf.status" == 1)
  
  ## births, deaths of old age, grim reaper deaths, total number infected, number infected via transmission, edge count, vertex count
  stats[time, "total_number_infected"] <- length(xn.infected)
  #stats[time, "infected_via_transmission"] <- length((xn.nw%v%"infector")[which(!is.na(xn.nw%v%"infector"))])
  stats[time, "edge_count"] <- network.edgecount(xn.nw) #num. of edges at last time
  stats[time, "vertex_count"] <- network.size(xn.nw) #vertex count at last time
  
  stats
}

for (r in 1:1) {
  set.seed(r)
  nw <- fit$network
  formation <- fit$formula
  dissolution <- ~offset(edges)
  popsize <- network.size(nw)
  
  timesteps <- 25
  start.time <- 1
  
  ## Start with all vertices and edges active
  nw <- activate.vertices(nw,onset=1,terminus=Inf)
  nw <- activate.edges(nw,onset=1,terminus=Inf)
  
  zeros <- rep(0, timesteps)
  stats <- data.frame(time=1:timesteps, births=zeros, old_age_deaths=zeros, grim_repear_deaths=zeros, 
                      total_number_infected=zeros, infected_via_transmission=zeros, edge_count=zeros, 
                      vertex_count=zeros)
  stats <- collect.stats(start.time)
  
  for (time in 2:timesteps) {
    
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
    
#     ret <- update.vital.dynamics(nw,
#                                 max.survival=50,
#                                 daily.death.prob=0.01/100,
#                                 daily.birth.rate=1/100
#                                 )
    #nw <- ret$network
    #stats <- ret$stats
    cat("Total number of edges is ",
        network.edgecount(nw), "\n")
    cat("Number of alive edges is ",
        network.edgecount(network.extract(nw, at=time)),
        "\n") 
  
    ret <- transmission(nw, verbose=TRUE,
                       like.age.prob=10/100,
                       unlike.age.prob=20/100,
                       )

    nw = ret$network
    stats[time, "infected_via_transmission"] <- ret$infected_count
    
    stats <- collect.stats(time)
    xn.nw <- network.collapse(nw, at=time)
    infected <- which(nw%v%"inf.status" == 1)
    xn.infected <- which(xn.nw%v%"inf.status" == 1)
    
    
    cat("Number of infected individuals, dead or alive, is ", length(infected), "\n")
    cat("Number of infected individuals, ALIVE, is ", length(xn.infected), "\n", "\n")
    
  }
  
  write.csv(stats, file=paste("data_", as.character(r), ".csv", sep=""), row.names = FALSE)
}

