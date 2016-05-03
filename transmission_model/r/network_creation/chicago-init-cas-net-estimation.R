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

   load(file="initialized-model.RData")
   source("../common/chicago_parameters.R")

   #####################
   ## MODEL SETUP
   formation <- ~edges+degree(0:1)

   dissolution <- ~offset(edges)
   theta.diss <- log(dur_cas - 1)
   target.stats <- c(cas_n_edges, cas_deg_seq[1:2]
                    )

   constraints <- ~.

   formation.n0 <- update.formula(formation, n0~.)

   #####################
   ## CREATE EMPTY NETWORK TO START
   n_cas <- network.initialize(n, directed=FALSE, bipartite=FALSE)

   #####################
   ## FIT MODEL
   cas_fit <- ergm(formation.n0, 
                   target.stats=target.stats, 
                   constraints=constraints,
                   eval.loglik=FALSE,
                   verbose=TRUE,
                   control=control.ergm(MCMLE.maxit=500)
                    )

   theta.form <- fit$coef 
   theta.form[1] <- theta.form[1] - theta.diss

   #####################
   ## SIMULATE (for testing)
#     hetdeg.diag.sim <- simulate(n0,
#                                 formation=formation.n0,
#                                 dissolution=dissolution,
#                                 coef.form=theta.form, 
#                                 coef.diss=theta.diss,
#                                 time.slices=2e4,
#                                 #time.slices=1e2,
#                                 constraints=constraints,
#                                 monitor=~edges+degree(0:5)
#                                 )

   #####################
#    ## TEST
#    net.f <- network.collapse(hetdeg.diag.sim, at=1000)
#    network.size(net.f)
#    network.edgecount(net.f)
#    degreedist(net.f) 

   #####################
   ## SAVE BINARY
   save.image(file="cas_net.RData")

   
