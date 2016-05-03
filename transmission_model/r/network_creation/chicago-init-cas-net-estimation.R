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
   formation_cas <- ~edges+degree(0:1)

   dissolution_cas <- ~offset(edges)
   theta.diss_cas <- log(dur_cas - 1)
   target.stats_cas <- c(cas_n_edges, cas_deg_seq[1:2]
                    )

   constraints_cas <- ~.

   formation.n_cas <- update.formula(formation_cas, n_cas~.)

   #####################
   ## CREATE EMPTY NETWORK TO START
   n_cas <- network.initialize(n, directed=FALSE, bipartite=FALSE)

   #####################
   ## FIT MODEL
   cas_fit <- ergm(formation.n_cas, 
                   target.stats=target.stats_cas, 
                   constraints=constraints_cas,
                   eval.loglik=FALSE,
                   verbose=TRUE,
                   control=control.ergm(MCMLE.maxit=500)
                    )

   theta.form_cas <- cas_fit$coef 
   theta.form_cas[1] <- theta.form_cas[1] - theta.diss_cas

   #####################
   ## SAVE BINARY
   save.image(file="cas_net.RData")

   
