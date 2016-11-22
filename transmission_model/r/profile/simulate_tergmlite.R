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


stergm_lite_sim <- function(net) {
  p <- stergm_prep(net, formation = formation.n0, dissolution=dissolution, 
                   coef.form=theta.form, coef.diss=theta.diss, constraints=constraints)
  el <- as.edgelist(net)
  attributes(el)$vnames <- NULL
  z <- simulate_network(p, el, coef.form=theta.form, coef.diss=theta.diss, save.changes=T)
}

time1 <- 1
time2 <- time1+100

##prof <-profvis({
  ##for (time in time1:time2){
    ##new_el <- stergm_lite_sim(cpp_net)
  ##}
##})
##print(prof)

  new_el_time <- list(time1:time2)
  for (time in time1:time2){
    new_el_time[[time]] <- stergm_lite_sim(cpp_net)
  }

save.image("tergmlite_profile.RData")


