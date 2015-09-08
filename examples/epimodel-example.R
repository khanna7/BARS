## example using EpiModel

## Packages and libraries
library(EpiModel)

## Packages and libraries
library(EpiModel)# 3 Sep 2013

   rm(list=ls())
   ## Example 1
   ## n=5000
   ## 3000 edges
   ## mean partnership duration = 10 timesteps
   ## simulation over 50 timesteps
   
   nw <- network.initialize(5000, directed = FALSE)
   formation <- ~edges
   target.stats <- c(3000)
   duration <- 10
   
   est_nw <- netest(nw, formation = formation, target.stats = target.stats,
                    coef.diss = dissolution_coefs(~offset(edges), duration=duration))
   est_nw
   
   dx <- netdx(est_nw, nsteps = 100, nsims = 1)
   dx
   
