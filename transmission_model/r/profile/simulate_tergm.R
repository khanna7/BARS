rm(list=ls())

suppressMessages(library(ergm))
suppressMessages(library(tergmLite))
suppressMessages(library(network))
suppressMessages(library(networkDynamic))
suppressMessages(library(profvis))
#suppressMessages(library(purrr))

#suppressMessages(library(parallel))
#load(file="../network_model/cas_net.RData")
#net <- readRDS("network_for_profiling.rds")
load(file="./for_profiling.RData")


  time1 <- 1
  time2 <- time1+100
  new_el_time <- list(time1:time2)

  for (time in time1:time2){
    new_el_time[[time]] <- simulate(net,
        
        formation = formation, 
        
        dissolution = dissolution,
        
        coef.form = theta.form, 
        
        coef.diss = theta.diss,
        
        constraints = constraints,
        
        #output = "changes",
        
        time.start = time,
        
        control = control.simulate.network()
	
	)
        
  }

save.image("tergm_profile.RData")


