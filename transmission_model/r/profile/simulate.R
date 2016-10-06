rm(list=ls())

suppressMessages(library(ergm))
suppressMessages(library(tergm))
suppressMessages(library(network))
suppressMessages(library(networkDynamic))
suppressMessages(library(profvis))

#suppressMessages(library(parallel))
load(file="../network_model/cas_net.RData")

time1 <- 1
time2 <- time1+0

profvis({
  for (time in time1:time2){
    
    net <- simulate(net,
                    
                    formation = formation, 
                    
                    dissolution = dissolution,
                    
                    coef.form = theta.form, 
                    
                    coef.diss = theta.diss,
                    
                    constraints = constraints,
                    
                    #output = "changes",
                    
                    time.start = time,
                    
                    control = control.simulate.network(),
                    
                    time.slices = 1
                    
    )
    
    
    cat("Entering transmission step at time", time,
        "of", time2, "\n")
    
  }
}
)