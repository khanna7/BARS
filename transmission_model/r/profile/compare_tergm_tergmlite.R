rm(list=ls())
## compare outputs from tergm and tergmLite

  library(network)
  library(networkDynamic)

  ## tergm
  load("tergm_profile.RData")
  tergm_out <- new_el_time
  tergm_nets <- lapply(new_el_time, as.network)
  tergm_mean_degs <- 2*unlist(lapply(tergm_nets, network.edgecount))/
                                unlist(lapply(tergm_nets, network.size))
  summary(tergm_mean_degs)

  ## tergmlite
  load("tergmlite_profile.RData")
  tergmlite_out <- new_el_time
  tergmlite_nets <- lapply(new_el_time, as.network)
  tergmlite_mean_degs <- 2*unlist(lapply(tergmlite_nets, network.edgecount))/
                                unlist(lapply(tergmlite_nets, network.size))
  summary(tergmlite_mean_degs)


  

