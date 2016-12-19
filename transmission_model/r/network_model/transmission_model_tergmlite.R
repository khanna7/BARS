rm(list=ls())


suppressMessages(library(ergm))
suppressMessages(library(tergmLite))
suppressMessages(library(network))
suppressMessages(library(networkDynamic))

load(file="../r/network_model/cas_net.RData")

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
cas_formation <- cas_fit$formula
cas_dissolution <- dissolution_cas

n_cas_simulate <- function(cas_net) {
  class(cas_net) <- "network"
  p <- stergm_prep(cas_net, formation = cas_formation, dissolution=cas_dissolution, 
                   coef.form=theta.form, coef.diss=theta.diss, constraints=constraints)
  el <- as.edgelist(cas_net)
  attributes(el)$vnames <- NULL
  z <- simulate_network(p, el, coef.form=theta.form, coef.diss=theta.diss, save.changes=T)
  return(attr(z, 'changes'))
}

nw_save <- function(net, fname, tick) {
  class(net) <- "network"
  set.network.attribute(net, "tick", tick)
  saveRDS(net, file=fname)
}
