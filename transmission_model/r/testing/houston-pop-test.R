# Houston population testing 

# Libraries ------

rm(list=ls())

library(ergm)
library(network)


# Data ------

load("../network_creation/houston_initialized-model_n5000.RData")
main.net <- net.f

load("../network_creation/houston_cas_net_n5000.RData")
cas.net <- net.f

# Test network statistics to targets ------

# network size and edgecounts
network.size(main.net); network.edgecount(main.net); nedges
network.size(cas.net); network.edgecount(cas.net); cas_n_edges

# degree distribution
degreedist(main.net); deg_seq
degreedist(cas.net); cas_deg_seq

# partnetship duration ??


# age difference
age <- main.net %v% "age"

age.main.p1 <- age[as.edgelist(main.net)[,1]]
age.main.p2 <- age[as.edgelist(main.net)[,2]]
summary(abs(age.main.p1 - age.main.p2))
summary(abs(sqrt(age.main.p1) - sqrt(age.main.p2)))

age.cas.p1 <- age[as.edgelist(cas.net)[,1]]
age.cas.p2 <- age[as.edgelist(cas.net)[,2]]
summary(abs(age.cas.p1 - age.cas.p2))
summary(abs(sqrt(age.cas.p1) - sqrt(age.cas.p2)))

# circumcision
circum <- main.net %v% "circum.status"

circum.main.p1 <- circum[as.edgelist(main.net)[,1]]
circum.main.p2 <- circum[as.edgelist(main.net)[,2]]


# Test network statistics to targets ------

