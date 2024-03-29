rm(list=ls())


suppressMessages(library(ergm))
suppressMessages(library(tergmLite))
suppressMessages(library(network))
suppressMessages(library(networkDynamic))

#load(file="../r/network_model/cas_net.RData")
#load(file="../r/network_model/cas_net_n5000.RData")
load("../r/network_model/houston_cas_net_n5000.RData")

nw <- fit$network
formation <- fit$formula
dissolution <- dissolution

nw_simulate <- function(net_for_sim) {
  class(net_for_sim) <- "network"
  p <- stergm_prep(net_for_sim, formation = formation, dissolution=dissolution, 
                   coef.form=theta.form, coef.diss=theta.diss, constraints=constraints)
  el <- as.edgelist(net_for_sim)
  attributes(el)$vnames <- NULL
  z <- simulate_network(p, el, coef.form=theta.form, coef.diss=theta.diss, save.changes=T)
  return(attr(z, 'changes'))
}

n_cas <- cas_fit$network

n_cas_simulate <- function(cas_net) {
  class(cas_net) <- "network"
  p <- stergm_prep(cas_net, formation = formation.n_cas, dissolution=dissolution_cas,
                   coef.form=theta.form_cas, coef.diss=theta.diss_cas, constraints=constraints_cas)
  el <- as.edgelist(cas_net)
  attributes(el)$vnames <- NULL
  z <- simulate_network(p, el, coef.form=theta.form_cas, coef.diss=theta.diss_cas, save.changes=T)
  return(attr(z, 'changes'))
}

nw_save <- function(net, fname, tick) {
  class(net) <- "network"
  set.network.attribute(net, "tick", tick)
  saveRDS(net, file=fname)
}
