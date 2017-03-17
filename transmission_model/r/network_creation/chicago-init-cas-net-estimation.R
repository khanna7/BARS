## initial network estimation
## For chicago, model in the last  version contained:
   ## density (mean edges), degree distribution, duration parameter
   ## add: age-mixing, sexual role, sero-sorting

   ## top matter
   rm(list=ls())

   library(ergm)
   library(network)
   library(networkDynamic)
   library(tergm)
   #library(parallel)

   load(file="initialized-model.RData")
   #source("../common/chicago_parameters.R")
   #np <- detectCores()

   #####################
   ## MODEL SETUP
   net <- fit$network
   formation_cas <- net~edges+degree(0:2)
                       

   dissolution_cas <- net~offset(edges)
   theta.diss_cas <- log(dur_cas - 1)
   target.stats_cas <- c(cas_n_edges,
                         cas_deg_seq[1:3]
                         )


   constraints_cas <- ~.
   formation.n_cas <- update.formula(formation_cas, net~.)

   ## sexual role
   role_casual <- sample(0:2, n, c(pr_versatile_main, 
                                 pr_insertive_main, 
                                 pr_receptive_main), 
                       replace=TRUE)
   
   table(role_casual, exclude=NULL)
   net %v% "role_casual" <- role_casual #0=versatile, 1=insertive, 2=receptive
   
   
   #####################
   ## CREATE EMPTY NETWORK TO START
   #n_cas <- network.initialize(n, directed=FALSE, bipartite=FALSE)

   cas_fit <- ergm(formation.n_cas, 
                   target.stats=target.stats_cas, 
                   constraints=constraints_cas,
                   eval.loglik=FALSE,
                   verbose=TRUE,
                   control=control.ergm(MCMLE.maxit=500)
                    )

   theta.form_cas <- cas_fit$coef 
   theta.form_cas[1] <- theta.form_cas[1] - theta.diss_cas

    cas_sim_test <- simulate(net,
                                formation=formation.n_cas,
                                dissolution=dissolution_cas,
                                coef.form=theta.form_cas, 
                                coef.diss=theta.diss_cas,
                                time.slices=2e4,
                                #time.slices=1e2,
                                constraints=constraints_cas,
                                monitor=~edges+degree(0:5))

   #####################
   ## TEST
   net.f <- network.collapse(cas_sim_test, at=1000)
   network.size(net.f)
   network.edgecount(net.f)
   degreedist(net.f) 
   
   #####################

   #####################
   ## SAVE BINARY
   save.image(file="cas_net.RData")

   
